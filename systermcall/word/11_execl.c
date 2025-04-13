#include "../Myhead.h"

int main(int argc, char* argv[])
{
    printf("You can see me!\n");
    printf("pid = %d\n", getpid());
    execl("add", "./add", "3", "4", NULL);
    printf("You can't see me!");
    return 0;
}