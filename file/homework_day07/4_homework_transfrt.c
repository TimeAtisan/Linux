#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
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
    int fd = open(argv[1], O_RDWR);
    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        perror("fstat error!\n");
        close(fd);
        return -1;
    }

    char* p = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ERROR_CHECK(p, NULL, "mmap error!\n");

    for (int i = 0; i < sb.st_size; i++)
    {
        if (p[i] >= 'a' && p[i] <= 'z')
        {
            p[i] -= 32;
        }
        else if (ispunct(p[i]))
        {
            p[i] = ' ';
        }
    }
    munmap(p, sb.st_size);
    close(fd);

    return 0;
}