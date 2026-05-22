#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/fb.h>
// 输入子系统对应输入事件的头文件
#include <linux/input.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3000 // 对方服务器的端口号
#define BUF_SIZE 1024
#define IP "10.11.53.4" // 对方服务器的IP地址

int main(int argc, char const *argv[])
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

    // 1.打开触摸屏，读取用户输入
    int t_fd = open("/dev/input/event0", O_RDONLY);

    if (t_fd == -1)
    {
        perror("open /dev/input/event0 failed!");
        return -1;
    }

    // 写一个死循环不断的读取屏幕文件的数据
    // 按照输入子系统标准创建 struct input_event 结构体变量去读取数据
    struct input_event evt;

    // 实现点击能力
    int x = -1, y = -1;
    while (1)
    {
        read(t_fd, &evt, sizeof(struct input_event));

        // printf("type %#x \n", evt.type);
        // printf("code %#x \n", evt.code);
        // printf("value %d \n", evt.value);

        if (evt.type == EV_ABS)
        {
            if (evt.code == ABS_X)
            {
                // printf("x:%d \n", evt.value * 800 / 1024);
                x = evt.value * 800 / 1024;
            }

            if (evt.code == ABS_Y)
            {
                // printf("y:%d \n", evt.value * 480 / 600);
                y = evt.value * 800 / 1024;
            }
        }

        // 手指送松开
        if (evt.type == EV_KEY && evt.code == BTN_TOUCH && evt.value == 0)
        {
            if (x != -1 && y != -1)
            {
                // printf("click x:%d y:%d \n", x, y);
                printf("开始录音 \n");
                int r = system("arecord -d3 -c1 -r16000 -twav  -fS16_LE ./ai_server2/bin/wav/2.wav");
                if (r != -1)
                {
                    printf("录制成功 \n");

                    write(sock, "录制成功", strlen("录制成功"));
                   

                    // 5. 读取服务端回复
                    read(sock, buffer, BUF_SIZE);
                    printf("Server: %s\n", buffer);
                }
            }
            // 还原为-1 方便给下一次点击做判断
            x = -1;
            y = -1;
        }
    }

    close(t_fd);
    // 6. 关闭
    close(sock);

    return 0;
}
