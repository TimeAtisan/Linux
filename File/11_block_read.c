#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fdr = open(argv[1], O_RDONLY);

    char buf[4096];

    sleep(30);

    printf("sleep over!\n");

    while (1)
    {
        read(fdr, buf, 4096);
        printf("read one!\n");
        sleep(2);
    }
    return 0;
}
