#include "../Myhead.h"

int main()
{
    printf("pid = %d bash pid = %d\n", getpid(), getppid());
    // pid = 10739 bash pid = 7666
    return 0;
}