#include <Myhead.h>

void handler(int signum)
{
    printf("[before:signum = %d]\n", signum);
    sleep(3);
    printf("[after:signum = %d]\n", signum);
}

int main(int argc, const char* argv[])
{
    struct sigaction act;

    memset(&act, 0, sizeof(act));
    act.sa_handler = handler;
    // act.sa_flags = SA_RESTART|SA_NODEFER; //
    // x信号递送过程中，没有屏蔽x这个信号，而是从新递送新的2号信号，递送完了再来递送原信号

    // act.sa_flags = SA_RESTART | SA_RESETHAND; // SA_RESETHAND信号只递送一次，再次到来不会递送

    act.sa_flags = SA_RESTART;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT);
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
    /*while (1)
    {
      sleep(1);
    }*/
    char buf[4096] = {0};
    ssize_t sret = read(STDIN_FILENO, buf, sizeof(buf));
    printf("sret = %ld,buf = %s\n", sret, buf);

    return 0;
}
