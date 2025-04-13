#include "../Myhead.h"

int main(int argc, char* argv[])
{
    pid_t pid = fork();
    if (0 == pid)
    {
        printf("[I am child,pid = %d,ppid = %d]\n", getpid(), getppid());
        sleep(3);
        return 0;
    }
    else
    {
        printf("[I am parents!]\n");
        pid_t cpid = wait(NULL);
        printf("[cpid = %d]\n", cpid);
        return 0;
    }

    return 0;
}