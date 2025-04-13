#include "../Myhead.h"

int main()
{
    if (fork())
    {
        printf("I am parent,pid = %d,pgid = %d,pgid of ppid = %d\n", getpid(), getppid(), getpgid(0));
    }
    else
    {
        printf("I am child,pid = %d,pgid = %d,pgid of ppid = %d\n", getpid(), getppid(), getpgid(0));
    }
    /*
      I am parent,pid = 14850,pgid = 7666,pgid of ppid = 14850
      I am child,pid = 14851,pgid = 14850,pgid of ppid = 14850
    */
    return 0;
}