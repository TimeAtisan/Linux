#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd, -1, "open");
    char buf[4096];
    bzero(buf, sizeof(buf));
    int ret = read(fd, buf, sizeof(buf));
    ERROR_CHECK(ret, -1, "read");
    printf("%s\n", buf);
    return 0;
}
