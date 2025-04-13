#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define ARGS_CHECK(argc, num)                 \
    {                                         \
        if (argc != num)                      \
        {                                     \
            fprintf(stdout, "args error!\n"); \
            return -1;                        \
        }                                     \
    }

#define ERROR_CHECK(ret, num, msg) \
    {                              \
        if (ret == num)            \
        {                          \
            perror(msg);           \
            return -1;             \
        }                          \
    }

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    printf("Hello world! 1\n");
    int fd_stdout = dup(STDOUT_FILENO);
    ERROR_CHECK(fd_stdout, -1, "dup error!\n");
    close(STDOUT_FILENO);
    int fd = open("pipe", O_WRONLY);
    ERROR_CHECK(fd, -1, "open error!\n");
    printf("Hello world! 2\n");
    int fd_ = dup2(fd_stdout, 1);
    ERROR_CHECK(fd_, -1, "dup2 error!");
    printf("Hello world! 3\n");

    close(fd_stdout);

    return 0;
}