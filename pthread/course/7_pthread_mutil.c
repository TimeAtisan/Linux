#include <Myhead.h>

void* threadFunc(void* arg)
{
    long pi = (long) arg;
    printf("child:pi = %ld\n", pi);

    return NULL;
}

int main(int argc, const char* argv[])
{
    long i = 1001;
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, threadFunc, (void*) i);
    ++i;
    pthread_create(&tid2, NULL, threadFunc, (void*) i);
    ++i;
    pthread_create(&tid3, NULL, threadFunc, (void*) i);
    sleep(1);
    printf("main: i = %ld\n", i);
    return 0;
}
