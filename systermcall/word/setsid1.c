#include "../Myhead.h"

int main()
{
    pid_t pid = fork();
    if (pid == 0)
    {
        printf("[child,pid = %d,ppid = %d,pgid = %d,sid = %d]\n", getpid(), getppid(), getpgid(0), getsid(0));
        int ret = setsid();
        ERROR_CHECK(ret, -1, "setsid"); // 注意不能是进程组组长
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
// 创建新会话以后，即使原来的shell被关闭了，子进程依然不受任何影响。