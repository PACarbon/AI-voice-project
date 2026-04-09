#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    // 打开文件
    int fd = open("/dev/fb0", O_RDWR);

    // 我们显示屏 800 * 480 像素
    // 每个像素四字节，分别是ARGB： Alpha透明度 Red红色 Green绿色 Blue蓝色 取值范围十六进制00~ff之间
    // 因此我们定义了一个显示屏显示颜色二维数组 表示480行 800列每个元素（像素）4字节
    int buf[480][800];

    // 写一个双重循环设置颜色
    for (int y = 0; y < 480; y++)
    {
        for (int x = 0; x < 800; x++)
        {
            buf[y][x] = 0xffffcc00; // Alpha:ff  Red:ff  Green:cc Blue:00
        }
    }

    write(fd, buf, 800 * 480 * 4);

    close(fd);

    return 0;
}
