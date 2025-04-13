#include "../Myhead.h"

int main()
{
    int wstatus;
    pid_t pid = fork();
    if (pid > 0)
    {
        wait(&wstatus);
        if (WIFEXITED(wstatus))
        {
            printf("exit status = %d\n", WEXITSTATUS(wstatus)); // 1
        }
        else if (WIFSIGNALED(wstatus))
        {
            printf("abnormal quit,terminal signal = %d\n", WTERMSIG(wstatus));
        }
    }
    else if (pid == 0)
    {
        sleep(100);
        return 1;
    }

    return 0;
}