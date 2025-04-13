#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int fd = open(argv[1], O_RDWR);
    ftruncate(fd, 698);

    char* p = (char*) mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    for (size_t i = 0; i < 498; i++)
    {
        if (p[i] >= 'a' && p[i] <= 'z')
        {
            p[i] -= 32;
        }
        else if (ispunct(i))
        {
            p[i] = ' ';
        }
    }
    munmap(p, 498);
    close(fd);

    return 0;
}
