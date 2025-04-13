#include <fcntl.h>
#include <stdio.h>
#include <string.h>
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
    // ARGS_CHECK(argc,2);
    char buf[127];
    int fd = open("pipe", O_RDONLY);
    memset(buf, 0, sizeof(buf));
    int ret = read(fd, buf, sizeof(buf));
    ERROR_CHECK(-1, ret, "read error!\n");
    printf("%s", buf);
    close(fd);

    return 0;
}