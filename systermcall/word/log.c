#include "../Myhead.h"

int main()
{
    int i = 0;
    if (fork() > 0)
    {
        printf("我是父进程,结束运行！\n");
        exit(0);
    }
    setsid();
    chdir("./");
    umask(0);
    for (; i < 64; ++i)
    {
        close(i);
    }
    i = 0;
    while (i < 10)
    {
        printf("%d\n", i);
        time_t ttime;
        time(&ttime);
        struct tm* pTm = gmtime(&ttime);
        syslog(LOG_INFO,
               "[%d %04d:%02d:%02d  %dmin %dsec]",
               i,
               (1900 + pTm->tm_year),
               (1 + pTm->tm_mon),
               (pTm->tm_mday),
               pTm->tm_min,
               pTm->tm_sec);
        i++;
        sleep(1);
    }

    return 0;
}