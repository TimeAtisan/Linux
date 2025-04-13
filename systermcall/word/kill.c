#include "../Myhead.h"

int main()
{
    pid_t pid = fork();
    int status;
    if (pid == 0)
    {
        printf("child,pid = %d,ppid = %d\n", getpid(), getppid());
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            printf("child exit code = %d\n", WEXITSTATUS(status));
            abort();
        }
        else if (WIFSIGNALED(status))
        {
            printf("child crash,signal = %d\n", WTERMSIG(status));
        }
    }
    return 0;
}