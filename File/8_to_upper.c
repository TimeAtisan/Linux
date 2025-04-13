#include <Myhead.h>

int main(int argc, char* argv[])
{
    int fd = open("file2", O_RDWR);
    ERROR_CHECK(fd, -1, "open");

    char buf[1024] = {0};
    int ret = read(fd, buf, sizeof(buf));
    ERROR_CHECK(ret, -1, "read");
    for (size_t i = 0; i < ret; i++)
    {
        if (buf[i] >= 'a' <= 'z')
        {
            buf[i] -= 32;
        }
    }
    lseek(fd, 0, SEEK_SET);
    ret = write(fd, buf, ret);
    ERROR_CHECK(ret, -1, "write");

    return 0;
}
