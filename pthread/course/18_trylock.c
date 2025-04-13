#include <Myhead.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* threadFunc(void* args)
{
    pthread_mutex_lock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, NULL);
    sleep(1);
    int ret = pthread_mutex_trylock(&mutex);
    THREAD_ERROR_CHECK(ret, "lock!\n");
    printf("[first lock!]\n");

    return 0;
}
