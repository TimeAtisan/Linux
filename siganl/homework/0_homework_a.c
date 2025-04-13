#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    pid_t pid = fork();
    ERROR_CHECK(pid, -1, "fork error!\n");
    if (pid > 0)
    {
        wait(NULL);
        puts("child is finish!\n");
    }
    else if (pid == 0)
    {
        fputs("I am child!\n", stdout);
        char buf[1024] = {0};
        sprintf(buf, "ls -a %s", argv[1]);
        execl("0_homework_b", "./0_homework_b", buf, NULL);
    }
    return 0;
}