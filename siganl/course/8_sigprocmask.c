#include <Myhead.h>

void handler(int signum)
{
    printf("signum = %d\n", signum);
}

int main(int argc, const char* argv[])
{
    signal(SIGINT, handler);

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigprocmask(SIG_BLOCK, &set, NULL);

    sleep(10);

    sigset_t pending;
    sigemptyset(&pending);
    sigpending(&pending);
    if (sigismember(&pending, SIGINT))
    {
        printf("SIGINT is pending\n");
    }
    else
    {
        printf("SIGINT is not pending\n");
    }
    sigprocmask(SIG_UNBLOCK, &set, NULL);
    sleep(3);
    return 0;
}