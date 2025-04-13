#include "../Myhead.h"

void Daemon()
{
    // 1.新建设备
    if (fork()) // 如果调用的是会话ID则不能调用
    {
        exit(0);
    }
    setsid();
    // 2.关闭设备
    for (int i = 0; i < 1024; ++i)
    {
        close(i);
    }

    // 3.修改环境属性
    chdir("./");
    umask(0);
}

int main()
{
    Daemon();
    for (int i = 0; i < 20; i++)
    {
        syslog(LOG_INFO, "I am daemon,i = %d\n ", i);
        sleep(2);
    }

    return 0;
}