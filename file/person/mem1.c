#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
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

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open error!");
    int fdw = open(argv[2], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open error!");
    fputs("I am char1\n", stdout);
    char buf[128];
    int ret = 0;

    while (1)
    {
        memset(buf, 0, sizeof(buf));
        ret = read(STDIN_FILENO, buf, sizeof(buf));
        ERROR_CHECK(ret, -1, "read error!\n");
        ret = write(fdw, buf, strlen(buf) - 1);
        ERROR_CHECK(ret, -1, "write error!\n");
        memset(buf, 0, sizeof(buf));
        ret = read(fdr, buf, sizeof(buf));
        fprintf(stdout, "%s\n", buf);
    }

    return 0;
}