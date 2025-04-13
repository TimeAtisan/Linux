#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fd = open("file1", O_RDWR);
    ERROR_CHECK(fd, -1, "open:");
    int ret = ftruncate(fd, 6);
    ERROR_CHECK(ret, -1, "ftruncate");
    char* p = mmap(NULL, 6, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ERROR_CHECK(p, MAP_FAILED, "mmap:");
    for (size_t i = 0; i < 6; i++)
    {
        printf("%c ", p[i]);
    }

    printf("\n");
    p[4] = 'O';
    printf("%s\n", p);

    munmap(p, 5);
    close(fd);

    return 0;
}
