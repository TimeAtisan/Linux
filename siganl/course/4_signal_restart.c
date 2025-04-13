#include <Myhead.h>

void handler(int signum)
{
    printf("signum = %d\n", signum);
}

int main(int argc, const char* argv[])
{
    signal(SIGINT, handler);
    char buf[4096] = {0};
    ssize_t sret = read(STDIN_FILENO, buf, sizeof(buf));
    printf("sret = %ld,buf = %s\n", sret, buf);
    return 0;
}