#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    if (fork())
    {
        sleep(1);
        write(fd, "world", 5);
    }
    else
    {
        write(fd, "hello ", 6);
    }
    return 0;
}
