#include <Myhead.h>

void* threadFunc(void* args)
{
    time_t now = time(NULL);
    char* p = NULL;
    char buf[1024] = {0};
    p = ctime_r(&now, buf);
    printf("Before:%s\n\n", p);
    sleep(5);
    printf("After: %s\n\n", p);
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, NULL);
    sleep(2);

    time_t now = time(NULL);
    char buf[1024] = {0};
    char* p = ctime_r(&now, buf);
    printf("Main p: %s\n\n", p);
    pthread_join(tid, NULL);

    return 0;
}
