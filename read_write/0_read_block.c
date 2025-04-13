#include <Myhead.h>

int main(int argc, char* argv[])
{
    // 读磁盘文件
    int fd = open("1.pipe", O_RDWR);
    char buf[3] = {0};
    while (1)
    {
        bzero(buf, sizeof(buf));
        ssize_t sret = read(fd, buf, 2);
        if (sret == 0)
        {
            break;
        }

        printf("sret = %ld,buf = %s\n", sret, buf);
        sleep(1);
    }

    return 0;
}
