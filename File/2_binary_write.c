#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");

    int i = 1000000;
    write(fd, &i, sizeof(i));
    close(fd);
    return 0;
}
