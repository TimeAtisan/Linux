#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define ERROR_CHECK(ret, num, msg) \
    {                              \
        if (ret == num)            \
        {                          \
            perror(msg);           \
            return -1;             \
        }                          \
    }
#define ARGS_CHECK(argc, num)                 \
    {                                         \
        if (argc != num)                      \
        {                                     \
            fprintf(stdout, "args error!\n"); \
            return -1;                        \
        }                                     \
    }

#define SIZE 128

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open error!");
    printf("fd = %d\n", fd);
    char buf[SIZE];
    int ret = 0;
    do
    {
        ret = read(fd, buf, SIZE);
        if (-1 == ret)
        {
            if (EINTR == errno)
            {
                continue;
            }
            exit(-1);
        }
        // write(STDOUT_FILENO,buf,ret);
        printf("%s", buf);
        // fprintf(STDERR_FILENO,"%s",buf);
        /*
        if (ret < SIZE) {
          buf[ret] = '\0';  // 手动在读取的字节后添加 '\0'，确保 buf 是一个有效的字符串
        }
        fprintf(stdout, "%s", buf);
        */
    } while (ret != 0);
    close(fd);
    return 0;
}