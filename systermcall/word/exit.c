#include "../Myhead.h"
// return和exit的区别
int func()
{
    printf("func!\n");
    // return 3;
    exit(3);
}

int main()
{
    pid_t pid = fork();
    if (0 == pid)
    {
        puts("child!");
        func();
        return 0;
    }
    else
    {
        fprintf(stdout, "parents!");
        int status;
        wait(&status);
        if (WIFEXITED(status))
        {
            printf("child exit code = %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}