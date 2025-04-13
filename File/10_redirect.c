#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    printf("you can see me!\n");
    // 隐藏bug,close之前要打印一个换行。
    close(STDOUT_FILENO);
    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    printf("fd = %d\n", fd);
    printf("you can not see me!\n");

    return 0;
}
