#include <Myhead.h>

int setNoblock(int fd)
{
    int flag = fcntl(fd, F_GETFL);
    flag |= O_NONBLOCK;
    int ret = fcntl(fd, F_SETFL);
    ERROR_CHECK(ret, -1, "fcntl");
    return 0;
}

int main(int argc, char* argv[])
{
    int fd = open("1.pipe", O_RDONLY);
    setNoblock(fd);
    char buf[3] = {0};
    while (1)
    {
        bzero(buf, sizeof(buf));
        ssize_t sret = read(fd, buf, sizeof(buf));
        printf("sret = %ld,buf = %s\n", sret, buf);
        sleep(1);
    }

    return 0;
}
