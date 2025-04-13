#include "../Myhead.h"

int main(int argc, char* argv[])
{
    pid_t pid = fork();
    if (pid == 0)
    {
        printf("I am child!\n");
        return 0;
    }
    else
    {
        printf("I am parents!\n");
        while (1)
            ;
    }
    return 0;
}