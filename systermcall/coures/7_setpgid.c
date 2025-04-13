#include "../Myhead.h"

int main()
{
    if (fork())
    {
        printf("Before:I am parent,pid = %d,ppid = %d,pgid  = %d\n", getpid(), getppid(), getpgid(0));

        printf("After:I am parent,pid = %d,pgid = %d,pgid = %d\n", getpid(), getppid(), getpgid(0));
        wait(NULL);
    }
    else
    {
        printf("Before:I am child,pid = %d,ppid = %d,pgid =  %d\n", getpid(), getppid(), getpgid(0));
        setpgid(0, 0);
        printf("After:I am child,pid = %d,ppid = %d,pgid = %d\n", getpid(), getppid(), getpgid(0));
        while (1)
        {
            sleep(1);
        }
    }
    /*
      I am parent,pid = 14850,pgid = 7666,pgid of ppid = 14850
      I am child,pid = 14851,pgid = 14850,pgid of ppid = 14850
    */
    return 0;
}