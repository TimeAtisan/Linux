#include <Myhead.h>

void* threadFunc(void* args)
{
    pthread_exit(NULL);
}
int main(int argc, char* argv[])
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_t tid;
    pthread_create(&tid, &attr, threadFunc, NULL);
    int ret = pthread_join(tid, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_join: ");

    return 0;
}
