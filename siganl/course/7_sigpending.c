#include <Myhead.h>

void handler(int signum)
{
    printf("[before:signum = %d]\n", signum);
    sleep(5);
    sigset_t pending;
    sigpending(&pending);
    if (sigismember(&pending, SIGINT))
    {
        printf("SIGINT is pending\n");
    }
    else
    {
        printf("SIGINT is not pending\n");
    }
    printf("[after:signum = %d]\n", signum);
}

int main(int argc, const char* argv[])
{
    signal(SIGINT, handler);
    char buf[4096] = {0};
    ssize_t sret = read(STDIN_FILENO, buf, sizeof(buf));
    printf("sret = %ld,buf = %s\n", sret, buf);
    return 0;
}