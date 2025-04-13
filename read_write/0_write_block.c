#include <Myhead.h>

int main(int argc, char* argv[])
{
    // 读磁盘文件
    int fd = open("1.pipe", O_RDWR);

    write(fd, "how are you!", sizeof("how are you!"));
    sleep(20);
    printf("sleep over!\n");
    close(fd);

    return 0;
}
