#include "../Myhead.h"

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int lhs = atoi(argv[1]);
    int rhs = atoi(argv[2]);
    printf("pid = %d,add = %d\n", getpid(), lhs + rhs);
    return 0;
}