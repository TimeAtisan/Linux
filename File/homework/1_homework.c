#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int fd1 = open(argv[1], O_RDONLY);
    int fd2 = open(argv[2], O_RDONLY);

    char buf1[4096];
    char buf2[4096];

    while (1)
    {
        bzero(buf1, sizeof(buf1));
        bzero(buf2, sizeof(buf2));

        ssize_t sret1 = read(fd1, buf1, sizeof(buf1));
        ssize_t sret2 = read(fd2, buf2, sizeof(buf2));
        if (sret1 != sret2)
        {
            printf("Not the same!\n");
            break;
        }
        else if (memcmp(buf1, buf2, sret1) != 0)
        {
            printf("Not the same!\n");
            break;
        }
        else if (sret1 == 0)
        {
            printf("The same!\n");
            break;
        }
    }

    return 0;
}
