#include <Myhead.h>

void handler(int signum)
{
    printf("signum = %d\n", signum);
}

int main(int argc, const char* argv[])
{
    signal(SIGINT, handler);
    pause();
    printf("byebye!\n");
    return 0;
}
