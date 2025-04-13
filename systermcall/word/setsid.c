#include "../Myhead.h"

int main()
{
    pid_t pid = fork();
    if (pid == 0)
    {
        printf("[child,pid = %d,ppid = %d,pgid = %d,sid = %d]\n", getpid(), getppid(), getpgid(0), getsid(0));
        setpgid(0, 0);
        printf("[child,pid = %d,ppid = %d,pgid = %d,sid = %d]\n", getpid(), getppid(), getpgid(0), getsid(0));
        while (1)
            ;
        exit(0);
    }
    else
    {
        printf("[parent,pid = %d,ppid = %d,pgid = %d,sid = %d]\n", getpid(), getppid(), getpgid(0), getsid(0));
        while (1)
            ;
        wait(NULL);
        exit(0);
    }
    return 0;
}
// 获取会话ID的例子