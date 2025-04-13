#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    mode_t mode;
    sscanf(argv[1], "%o", &mode);
    int ret = chmod(argv[2], mode);
    ERROR_CHECK(ret, -1, "chmod");
    struct dirent dir;
    return 0;
}
