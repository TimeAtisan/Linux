#include <Myhead.h>
void handler(int signum)
{
    printf("[signum = %d]\n", signum);
}

int main(int argc, char* argv[])
{
    sleep(10);               // 10秒内没有信号注册，Ctrl+C 直接终止，10秒后就不会终止，执行handler
    signal(SIGINT, handler); // signal不会阻塞
    while (1)
    {
        sleep(1);
    }

    return 0;
}