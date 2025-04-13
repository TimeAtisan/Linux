#include <Myhead.h>
void handler(int signum)
{
    time_t now = time(NULL);

    printf("signum = %d,now = %s", signum, ctime(&now));
}
int main(int argc, const char* argv[])
{
    signal(SIGVTALRM, handler);
    struct itimerval itimer;
    itimer.it_value.tv_sec = 3;
    itimer.it_value.tv_usec = 0;

    itimer.it_interval.tv_sec = 1;
    itimer.it_interval.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &itimer, NULL);
    while (1)
        ; // 一直在占用CPU
    /*while (1)
    {
      sleep(1);
    } */// 没有占用CPU

    return 0;
}
