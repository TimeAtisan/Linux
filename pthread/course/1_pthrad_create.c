#include <Myhead.h>

void* routine(void* arg)
{
    printf("I am child thread!\n");
    return NULL;
}

int main(int argc, const char* argv[])
{
    pthread_t tid; // 将来获取子线程的tid
    int ret = pthread_create(&tid, NULL, routine, NULL);
    if (ret != 0)
    {
        fprintf(stderr, "%s%s", "pthread_create:", strerror(ret));
    }
    printf("I am main thread!\n");
    sleep(1);
    return 0;
}
