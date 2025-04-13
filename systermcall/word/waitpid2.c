#include "../Myhead.h"

int main()
{
    pid_t pid = fork();
    int status = 0;
    if (pid == 0)
    {
        printf("child,pid = %d,ppid = %d\n", getpid(), getppid());
        // sleep(5);
        return 123;
    }
    else if (pid > 0)
    {
        printf("parent,pid = %d,ppid = %d\n", getpid(), getppid());
        sleep(1);
        int ret = waitpid(pid, &status, WNOHANG);
        if (ret > 0)
        {
            if (WIFEXITED(status))
            {
                printf("child exit code = %d\n", WEXITSTATUS(status));
            }
            else if (WIFSIGNALED(status))
            {
                printf("child crash siganl = %d\n", WTERMSIG(status));
            }
        }
        printf("ret = %d\n", ret);
    }

    return 0;
}