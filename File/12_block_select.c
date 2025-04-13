#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fd1 = open(argv[1], O_RDWR);
    int fd2 = open(argv[1], O_RDWR);
    printf("pipe is opened!\n");

    fd_set rdset;
    fd_set wrset;
    int cnt = 0;
    char buf[4096] = {0};
    while (1)
    {
        FD_ZERO(&rdset);
        FD_ZERO(&wrset);
        FD_SET(fd1, &wrset);
        FD_SET(fd2, &rdset);

        int ret = select(fd2 + 1, &rdset, &wrset, NULL, NULL);
        printf("-----------2------------\n");
        if (FD_ISSET(fd2, &rdset))
        {
            printf("read ready,cnt = %d ret = %d\n", cnt++, ret);
            read(fd2, buf, 2048);
        }

        if (FD_ISSET(fd1, &wrset))
        {
            printf("write ready,cnt = %d ret = %d\n", cnt++, ret);
            write(fd1, buf, 4096);
        }
        sleep(1);
    }

    return 0;
}
