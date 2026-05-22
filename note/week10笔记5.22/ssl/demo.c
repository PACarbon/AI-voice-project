#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/*

# 证书 Debian/Ubuntu
sudo apt-get install ca-certificates

// 引入OpenSSL库
// sudo apt-get install libssl-dev

// gcc demo.c -o https  -lssl -lcrypto

*/

// 引入OpenSSL库
#include <openssl/ssl.h>
#include <openssl/err.h>
// #include "cJSON.h"

// https 默认端口号就是443
#define PORT 443
#define INTERFACE_NAME "api.deepseek.com"
#define PATH "/chat/completions"
#define AUTHORIZATION "你自己的key"


// OpenSSL 错误码定义
#define MY_SSL_ERROR_NONE            0
#define MY_SSL_ERROR_ZERO_RETURN     5  // ✅ 正常关闭连接，不是错误！

// 定义是否启用证书验证（0表示禁用，1表示启用）
#define VERIFY_CERTIFICATE 0

char SERVER_IP[INET_ADDRSTRLEN] = {0};
int sock = 0;
SSL *ssl = NULL;
SSL_CTX *ctx = NULL;

// 初始化OpenSSL
static void init_openssl()
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

// 清理OpenSSL资源
static void cleanup_openssl()
{
    EVP_cleanup();
    ERR_free_strings();
}

// 创建SSL上下文
static SSL_CTX *create_context()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    if (!ctx)
    {
        printf("Unable to create SSL context\n");
        ERR_print_errors_fp(stderr);
        return NULL;
    }

    return ctx;
}

// 配置SSL上下文
static void configure_context(SSL_CTX *ctx)
{
    #if VERIFY_CERTIFICATE
    // 启用证书验证
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);

    // 设置验证深度
    SSL_CTX_set_verify_depth(ctx, 5);

    // 尝试加载系统默认的CA证书
    if (!SSL_CTX_set_default_verify_paths(ctx))
    {
        printf("Warning: Failed to set default verify paths\n");
        // 尝试指定常见的CA证书路径
        if (!SSL_CTX_load_verify_locations(ctx, "/etc/ssl/certs/ca-certificates.crt", NULL))
        {
            printf("Warning: Failed to load CA certificates from /etc/ssl/certs/ca-certificates.crt\n");
        }
    }
    #else
    // 禁用证书验证（仅用于测试！）
    SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);
    #endif
}

// 1. 初始化 socket 连接和SSL
static int init_socket_and_ssl()
{
    struct sockaddr_in serv_addr;

    // 初始化socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;

    // 将 IP 地址从字符串转换为二进制格式
    struct hostent *p;
    p = gethostbyname(INTERFACE_NAME);
    if (p == NULL)
    {
        printf("\n Hostname resolution error \n");
        close(sock);
        return -2;
    }

    // 专门用来存储ip地址的数组
    char ipbuf[20] = {0};
    strcpy(ipbuf, inet_ntoa(*(struct in_addr *)((p->h_addr_list)[0])));
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(ipbuf);

    printf("deepseek 对应的ip地址是 %s \n", ipbuf);

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        close(sock);
        return -3;
    }

    // 初始化OpenSSL
    init_openssl();

    // 创建SSL上下文
    ctx = create_context();
    if (!ctx)
    {
        close(sock);
        return -4;
    }

    // 配置SSL上下文
    configure_context(ctx);

    // 创建SSL连接
    ssl = SSL_new(ctx);
    if (!ssl)
    {
        printf("Unable to create SSL connection\n");
        SSL_CTX_free(ctx);
        close(sock);
        return -5;
    }

    // 将SSL与socket关联
    if (SSL_set_fd(ssl, sock) != 1)
    {
        printf("Error setting SSL file descriptor\n");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(sock);
        return -6;
    }

    // 执行SSL握手
    printf("Performing SSL handshake...\n");
    int ret = SSL_connect(ssl);
    if (ret != 1)
    {
        int err = SSL_get_error(ssl, ret);
        printf("SSL connection failed: %d\n", err);
        ERR_print_errors_fp(stderr);

        #if VERIFY_CERTIFICATE
        printf("\n\nTIP: If this is a certificate verification issue, you can try:\n");
        printf("1. Install the ca-certificates package on your system\n");
        printf("2. Try temporarily disabling certificate verification by setting VERIFY_CERTIFICATE to 0\n");
        printf("   (Warning: This is insecure and should only be used for testing)\n");
        #endif

        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(sock);
        return -7;
    }

    printf("SSL connection established successfully\n");

    #if VERIFY_CERTIFICATE
    // 验证证书的域名
    X509 *cert = SSL_get_peer_certificate(ssl);
    if (cert)
    {
        printf("Certificate verification successful\n");
        X509_free(cert);
    }
    else
    {
        printf("Warning: No certificate was presented by the server\n");
    }
    #endif

    return sock;
}

// 2. 构建HTTP请求
static char* build_http_request(const char* user_input)
{
    const char* authorization =  AUTHORIZATION;
    
    int input_length = strlen(user_input);
    // 计算 request_body 的长度，包含固定部分和用户输入部分
    int body_length = strlen("{\"model\": \"deepseek-v4-flash\", \"messages\": [ {\"role\": \"user\", \"content\": \"\"}], \"thinking\": {\"type\": \"enabled\"}, \"reasoning_effort\": \"high\", \"stream\": false}") + input_length;
    char* request_body = (char*)malloc(body_length + 1);
    if (request_body == NULL)
    {
        printf("\n Memory allocation error for request_body \n");
        return NULL;
    }
    // 构造 request_body
    sprintf(request_body, "{\"model\": \"deepseek-v4-flash\", \"messages\": [ {\"role\": \"user\", \"content\": \"%s\"}], \"thinking\": {\"type\": \"enabled\"}, \"reasoning_effort\": \"high\", \"stream\": false}", user_input);

    char* request = (char*)malloc(1024 + body_length);
    if (request == NULL)
    {
        printf("\n Memory allocation error for request \n");
        free(request_body);
        return NULL;
    }

    sprintf(request, "POST %s HTTP/1.1\r\n", PATH);
    sprintf(request + strlen(request), "Host: %s\r\n", INTERFACE_NAME);
    sprintf(request + strlen(request), "Content-Type: application/json\r\n");
    sprintf(request + strlen(request), "Authorization: Bearer %s\r\n", authorization);
    sprintf(request + strlen(request), "Content-Length: %d\r\n", body_length);
    sprintf(request + strlen(request), "Connection: close\r\n\r\n");
    strcat(request, request_body);
    
    free(request_body);
    return request;
}

// 3. 发送HTTP请求并接收响应
static void send_request_and_receive_response(const char* user_input)
{
    char* request = build_http_request(user_input);
    if (request == NULL)
    {
        return;
    }

    // 发送请求
    int sent = SSL_write(ssl, request, strlen(request));
    if (sent <= 0)
    {
        int err = SSL_get_error(ssl, sent);
        printf("\n Send request error: %d\n", err);
        free(request);
        return;
    }
    printf("Sent %d bytes\n", sent);
    free(request);

    // 接收响应
    char buffer[4096];
    char response[4096 * 10] = {0};  // 假设响应最大为40960字节
    int bytes_received;
    int total_bytes = 0;

    printf("Receiving response...\n");
    while ((bytes_received = SSL_read(ssl, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_received] = '\0';
        strncat(response, buffer, bytes_received);
        total_bytes += bytes_received;
    }

    int err = SSL_get_error(ssl, bytes_received);
    // ✅ 正确判断：只有 非0 且 非5 才是真错误
    if (err != MY_SSL_ERROR_NONE && err != MY_SSL_ERROR_ZERO_RETURN)
    {
        printf("\n Receive response error: %d\n", err);
    }
    // ✅ SSL_ERROR_ZERO_RETURN(5) = 对端正常关闭连接，数据已收完
    else if (err == MY_SSL_ERROR_ZERO_RETURN)
    {
        printf("\n Connection closed normally by peer\n");
    }
    printf("Received total %d bytes\n", total_bytes);

    printf("response: %s \n", response);

    /*
    // 提取JSON部分
    char *json_start = strstr(response, "{");
    if (json_start == NULL)
    {
        printf("No JSON data found in the response.\n");
        return;
    }

    // 解析JSON数据
    cJSON *root = cJSON_Parse(json_start);
    if (root == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return;
    }

    // 获取choices数组
    cJSON *choices = cJSON_GetObjectItemCaseSensitive(root, "choices");
    if (cJSON_IsArray(choices))
    {
        // 获取数组的第一个元素
        cJSON *first_choice = cJSON_GetArrayItem(choices, 0);
        if (first_choice != NULL)
        {
            // 获取message对象
            cJSON *message = cJSON_GetObjectItemCaseSensitive(first_choice, "message");
            if (message != NULL)
            {
                // 获取content字段
                cJSON *content = cJSON_GetObjectItemCaseSensitive(message, "content");
                if (cJSON_IsString(content) && content->valuestring != NULL)
                {
                    // 打印回复内容
                    printf("回复内容: %s\n", content->valuestring);
                }
            }
        }
    }

    // 释放JSON对象
    cJSON_Delete(root);
    */
}

int main(int argc, char const *argv[])
{
    printf("Starting DeepSeek API client...\n");

    if (init_socket_and_ssl() < 0)
    {
        printf("Failed to initialize socket and SSL\n");
        return 1;
    }

    send_request_and_receive_response("我想创业ai很火我想卖token,该如何操作?");

    // 清理资源woxiangchuang
    printf("Cleaning up resources...\n");

    if (ssl)
    {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }

    if (ctx)
    {
        SSL_CTX_free(ctx);
    }

    cleanup_openssl();

    // 关闭socket
    close(sock);

    printf("Client finished successfully\n");
    return 0;
}