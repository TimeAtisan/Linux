#include <Myhead.h>

int main(int argc, char* argv[])
{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    printf("[first lock!]\n");
    pthread_mutex_lock(&mutex);
    printf("[second lock!]\n");
    return 0;
}
