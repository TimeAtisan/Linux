#include <Myhead.h>
void handler(int signum)
{
    printf("[Before:signum = %d]\n", signum);
    sleep(5);
    printf("[After:signum = %d]\n", signum);
}

int main(int argc, char* argv[])
{
    signal(SIGINT, handler); // signal不会阻塞
    while (1)
    {
        sleep(1);
    }

    return 0;
}