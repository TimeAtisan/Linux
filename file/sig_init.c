#include <Myhead.h>

void handler(int signum)
{
    printf("signum = %d is commming!\n", signum);
}

int main(int argc, char* argv[])
{
    signal(SIGINT, handler);
    printf("proces begin!\n");
    while (1)
    {
        sleep(1);
    }

    return 0;
}
