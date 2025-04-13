#include "../Myhead.h"

int main()
{
    pid_t pid = fork();
    int status;
    if (0 == pid)
    {
        printf("child,pid = %d ppid = %d\n", getpid(), getppid());
        char* p = NULL;
        *p = 'a';
        return 123;
    }
    else if (pid > 0)
    {
        printf("parent,pid = %d,ppid = %d\n", getpid(), getppid());
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            printf("child exit code = %d\n", WIFEXITED(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("child crash,signal = %d\n", WTERMSIG(status));
        }
    }
}
