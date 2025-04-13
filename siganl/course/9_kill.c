#include <Myhead.h>

int main(int argc, const char* argv[])
{
    // ./9_kill 12802
    ARGS_CHECK(argc, 2);
    kill(atoi(argv[1]), 9);
    return 0;
}
