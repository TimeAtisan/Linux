#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define ARGS_CHECK(argc, num)                        \
    {                                                \
        if (num != argc)                             \
        {                                            \
            fprintf(stdout, "%s", "args is error!"); \
            return -1;                               \
        }                                            \
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

    if (fork())
    {
        int fd = open(argv[1], O_RDWR);
        ERROR_CHECK(fd, -1, "open error!");

        write(fd, "YH!\n", 4);
    }
    else
    {
        int fd = open(argv[1], O_RDWR);
        ERROR_CHECK(fd, -1, "open error!");
        sleep(1);
        write(fd, "hello ", 6);
    }
    return 0;
}