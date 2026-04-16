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

    // 画图拿到图片的像素
    // 1. 打开图片
    int bmp_fd = open("1.bmp", O_RDWR);
    // 2. 读取图片像素， 注意 bmp 图片前54个字节是bmp图片相关信息并不是像素，我们不需要，所以在读取时要跳过54字节
    // 跳转到文件开头（偏移 54 字节）
    lseek(bmp_fd, 54, SEEK_SET);

    // 3. 创建存储bmp像素的数组，我们的bmp图24位图，一像素 3 字节，c语言没有3字节数据类型，退而求其次使用char（一字节）类型读取
    char bmp_arr[800 * 480 * 3];

    // 4. 读取
    int ret = read(bmp_fd, bmp_arr, 800 * 480 * 3);
    if (ret != 800 * 480 * 3)
    {
        perror("read bmp failed!");
        return -1;
    }

      for (int y = 0; y < 480; y++)
    {
        for (int x = 0; x < 800; x++)
        {
            // 二维坐标转以为坐标
            int i = y*800 + x;
            // BMP图片像素 每三个元素是一个像素对应的rgb
            char b = bmp_arr[i * 3 + 0];
            char g = bmp_arr[i * 3 + 1];
            char r = bmp_arr[i * 3 + 2];
            // (479 - y) 解决上下颠倒
            buf[(479 - y) * 800 + x] = 0xff000000 | r << 16 | g << 8 | b;
        }
    }
    

    // 取消内存映射
    munmap(buf, 800 * 480 * 4);
    close(fd);
    close(bmp_fd);

    return 0;
}
