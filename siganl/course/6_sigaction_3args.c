#include <Myhead.h>

void handler(int signum, siginfo_t* siginfo, void* p) // handler更换成3个参数的
{
    printf("[before:signum = %d,uid = %d,pid = %d]\n", signum, siginfo->si_uid, siginfo->si_pid);

    // printf("[after:signum = %d]\n",signum);
}

int main(int argc, const char* argv[])
{
    struct sigaction act;

    memset(&act, 0, sizeof(act));
    act.sa_sigaction = handler; // 结构体成员是sa_sigaction而不是sa_handler

    act.sa_flags = SA_RESTART | SA_SIGINFO; // step1 加上 SA_SIGINFO
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
