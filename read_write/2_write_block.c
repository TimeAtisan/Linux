#include <Myhead.h>

int main(int argc, char* argv[])
{
    int fd = open("1.pipe", O_RDWR);
    write(fd, "hello world!", sizeof("hello world!"));
    sleep(20);
    printf("sleep over!\n");
    close(fd);
    return 0;
}
