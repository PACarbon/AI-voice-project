#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3000 // 对方服务器的端口号
#define BUF_SIZE 1024
#define IP "169.254.10.68" // 对方服务器的IP地址

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr; // 存放对方服务器ip相关数据的结构体
    char buffer[BUF_SIZE] = {0};
    char message[BUF_SIZE] = {0};

    // 1. 创建 socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 2. 转换 IP 地址
    if (inet_pton(AF_INET, IP, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/Address not supported\n");
        return -1;
    }

    // 3. 连接服务端
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Connection failed");
        return -1;
    }
    while (1)
    {
    
        scanf("%s", message);
        // 4. 向服务器发送数据
        write(sock, message, strlen(message));
        printf("Client message sent: %s\n", message);

        // 5. 读取服务端回复
        read(sock, buffer, BUF_SIZE);
        printf("Server: %s\n", buffer);
    }

    // 6. 关闭
    close(sock);

    return 0;
}