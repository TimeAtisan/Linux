#include <Myhead.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* threadFunc(void* args)
{
    printf("child:I have lock!\n");
    pthread_mutex_lock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, NULL);
    sleep(1);
    pthread_mutex_lock(&mutex);
    printf("I got lock!\n");
    pthread_mutex_unlock(&mutex);
    pthread_join(tid, NULL);

    return 0;
}
