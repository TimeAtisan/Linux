#include <Myhead.h>

int main(int argc, char* argv[])
{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    printf("[first lock!]\n");
    int ret = pthread_mutex_trylock(&mutex);
    THREAD_ERROR_CHECK(ret, "lock!\n");
    printf("[second lock!]\n");
    return 0;
}
