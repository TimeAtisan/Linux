#include "../Myhead.h"
int func()
{
    printf("func"); // 这里不要添加换行符
    // exit(3);
    _exit(3); // 这种情况func不会打印
}
int main()
{
    pid_t pid = fork();
    if (pid == 0)
    {
        puts("child");
        func();
        return 0;
    }
    else
    {
        puts("parent");
        int status;
        wait(&status);
        if (WIFEXITED(status))
        {
            printf("child exit code = %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}