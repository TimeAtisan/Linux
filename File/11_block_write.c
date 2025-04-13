#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fdw = open(argv[1], O_WRONLY);

    char buf[4096] = {0};

    int cnt = 0;

    while (1)
    {
        write(fdw, buf, sizeof(buf));
        printf("write one,cnt = %d\n", cnt++);
    }
    return 0;
}
