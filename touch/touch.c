#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <linux/input.h> //触摸屏的头文件

int main()
{
    // 1. 打开触摸屏
    int tfd = open("/dev/input/event0", O_RDONLY);
    if (tfd == -1)
    {
        perror("open /dev/input/event0 failed! \n");
        return -1;
    }

    printf("打开触摸屏成功! \n");

    struct input_event info;
    int x = -1, y = -1;

    while (1)
    {
        read(tfd, &info, sizeof(info));

        if (info.type == EV_KEY && info.code == BTN_TOUCH)
        {
            if (info.value == 1)  // 按下
            {
                printf("屏幕按下 \n");
            }
            else  // 松开
            {
                if (x != -1 && y != -1)  // 确保点击有效
                {
                    printf("点击坐标: x = %d, y = %d \n", x, y);
                }
                else
                {
                    printf("无效点击!\n");
                }
                x = -1;  // 重置坐标
                y = -1;
            }
        }

        if (info.type == EV_ABS)
        {
            if (info.code == ABS_X)
            {
                x = info.value;
            }

            if (info.code == ABS_Y)
            {
                y = info.value;
            }
        }
    }

    close(tfd);
}


