#include "../Myhead.h"

int main()
{
    pid_t pid = fork();
    if (0 == pid)
    {
        printf("child,pid = %d,ppid = %d,pgid = %d\n", getpid(), getppid(), getpgrp());
    }
    else
    {
        printf("parent,pid = %d,ppid = %d,pgid = %d\n", getpid(), getppid(), getpgid(0));
        wait(NULL);
        exit(0);
    }
    return 0;
}