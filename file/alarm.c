#include <Myhead.h>

void handler(int sig)
{
    printf("sig %d,commming!\n", sig);
    time_t now = time(NULL);
    printf("%s\n", ctime(&now));
}
int main(int argc, char* argv[])
{
    signal(SIGALRM, handler);
    time_t now = time(NULL);
    printf("%s\n", ctime(&now));
    alarm(5);
    pause();
    return 0;
}
