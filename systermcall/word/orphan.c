#include "../Myhead.h"

int main(int argc, char* argv[])
{
    pid_t pid = fork();
    if (pid == 0)
    {
        sleep(100);
        printf("I am child!\n");
    }
    else
    {
        printf("I am parents!\n");
        return 0;
    }

    return 0;
}