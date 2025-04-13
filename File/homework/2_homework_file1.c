#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    // char ch = '1';
    char buf[10000];
    memset(buf, '1', sizeof(buf));
    for (size_t i = 0; i < 100; i++)
    {
        write(fd, buf, sizeof(buf));
    }
    close(fd);
    return 0;
}
