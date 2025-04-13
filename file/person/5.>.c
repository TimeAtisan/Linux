#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ARGS_CHECK(argc, num)                 \
    {                                         \
        if (argc != num)                      \
        {                                     \
            fprintf(stdout, "agrs error!\n"); \
            return -1;                        \
        }                                     \
    }

#define ERROR_CHECK(ret, num, msg) \
    {                              \
        if (num == ret)            \
        {                          \
            perror(msg);           \
            return -1;             \
        }                          \
    }

// 更加复杂的例子
int main(int argc, char* argv[])
{
    // ./redirect file1
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_WRONLY);
    ERROR_CHECK(fd, -1, "open");
    printf("我过来啦！\n");
    int newfd = 10;
    dup2(STDOUT_FILENO, newfd); // 使用newfd备份输出文件对象
    // 让1引用磁盘文件
    dup2(fd, STDOUT_FILENO);
    printf("我过去啦！\n");
    // 让1引用输出设备
    dup2(newfd, STDOUT_FILENO);
    printf("我又回来啦！\n");
    close(fd);
    return 0;
}