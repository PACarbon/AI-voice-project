#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char const *argv[])
{
    // 想要内存映射必须先打开对应的设备文件
    // 1.打开lcd屏幕
    int fd = open("/dev/fb0", O_RDWR);

    if (fd == -1)
    {
        perror("open /dev/fb0 failed!");
        // 打开失败退出主程序
        return -1;
    }
    // buf 其实就是 int arr[800 * 480] 一维数组的指针
    int *buf = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // 内存映射建立完毕后，就无需使用文件io的读写方法了，上面buf数组就直接变成了文件的映射，你修改buf就等于直接修改lcd屏幕
    for (int y = 0; y < 480; y++)
    {
        for (int x = 0; x < 800; x++)
        {
            int color = 0xfffcff00; // 黄色

            if (y <= 160) // 德国国旗黑色
            {
                color = 0xff000000;
            }
            else if (y <= 2 * 160) // 德国国旗红色
            {
                color = 0xffb70000;
            }
            // 推导出来的坐标转1维公式
            buf[y * 800 + x] = color;
        }
    }

    sleep(1); // 程序暂停1秒

    for (int y = 0; y < 480; y++)
    {
        for (int x = 0; x < 800; x++)
        {
            int color = 0xffb70000; // 深红

            if (x <= 266) // 法国国旗蓝色
            {
                color = 0xff3f48cc;
            }
            else if (x <= 2 * 266) // 法国国旗白色
            {
                color = 0xffffffff;
            }

            buf[y * 800 + x] = color;
        }
    }

    // 取消内存映射
    munmap(buf, 800 * 480 * 4);
    close(fd);

    return 0;
}
