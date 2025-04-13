#include <Myhead.h>

int gloabl = 1000;

void* threadFunc(void* args)
{
    printf("child:gloabl = %d\n", gloabl++);
    return NULL;
}

int main(int argc, const char* argv[])
{
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_create:");
    sleep(1);
    printf("main:gloabl = %d\n", gloabl);
    return 0;
}
