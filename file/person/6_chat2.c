#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
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
    int fdw = open(argv[1], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open error!\n");
    int fdr = open(argv[2], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open error!\n");
    fprintf(stdout, "chat is established!\n");
    fd_set readset;
    char buf[4096];
    int ret = 0;
    while (1)
    {
        FD_ZERO(&readset);
        FD_SET(STDIN_FILENO, &readset);
        FD_SET(fdr, &readset);
        select(fdr + 1, &readset, NULL, NULL, NULL);
        if (FD_ISSET(fdr, &readset))
        {
            memset(buf, 0, sizeof(buf));
            ret = read(fdr, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read error!");
            if (ret == 0)
            {
                printf("hehe!\n");
                break;
            }
            fprintf(stdout, "%s\n", buf);
        }

        if (FD_ISSET(STDIN_FILENO, &readset))
        {
            memset(buf, 0, sizeof(buf));
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read error!\n");
            if (ret == 0)
            {
                write(fdw, "ni shi ge hao ren!\n", 20);
                break;
            }
            write(fdw, buf, ret);
        }
    }
    close(fdr);
    close(fdw);

    return 0;
}