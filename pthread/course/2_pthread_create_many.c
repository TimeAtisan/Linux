#include <Myhead.h>

void* threadFunc(void* arg)
{
    while (1)
    {
        sleep(1);
    }
}

int main(int argc, const char* argv[])
{
    int cnt = 0;
    while (1)
    {
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, threadFunc, NULL);
        THREAD_ERROR_CHECK(ret, "pthread_create:");
        if (ret != 0)
        {
            printf("cnt = %d\n", cnt);
            break;
        }
        ++cnt;
    }

    return 0;
}
