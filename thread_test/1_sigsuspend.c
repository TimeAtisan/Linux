#include <Myhead.h>
void handler(int signum)
{
}
int mysleep(int time)
{
    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, NULL);

    sigset_t blset, oldset;
    sigemptyset(&blset);
    sigemptyset(&oldset);
    sigaddset(&blset, SIGALRM);
    sigprocmask(SIG_BLOCK, &blset, NULL);

    alarm(time);
    sigsuspend(&oldset);

    sigprocmask(SIG_UNBLOCK, &blset, NULL);
}

int main(int argc, char* argv[])
{
    time_t now = time(NULL);
    printf("%s\n", ctime(&now));

    mysleep(1);
    now = time(NULL);
    printf("%s\n", ctime(&now));

    mysleep(3);
    now = time(NULL);
    printf("%s\n", ctime(&now));

    return 0;
}
