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
    int fd = open(argv[1], O_RDWR);
    char buf1[100000];
    char buf2[100000];
    int i = 1;
    while (i < 100000)
    {
        buf1[i - 1] = 'A';
        buf2[i - 1] = 'B';
        i++;
    }

    if (fork())
    {
        write(fd, buf1, strlen(buf1));
    }
    else
    {
        write(fd, buf2, strlen(buf2));
    }
    return 0;
}

// 先open在fork