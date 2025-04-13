#include <Myhead.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct shareRes_s
{
    int flag; // flag 0-->A 完成未完成 1--->A 已经完成
    pthread_mutex_t mutex;
} shareRes_t;
void A()
{
    printf("Before: A\n");
    sleep(2);
    printf("After: A\n");
}

void B()
{
    printf("Before: B\n");
    sleep(2);
    printf("After: B\n");
}
void* threadFunc(void* args)
{
    shareRes_t* pshareRes = (shareRes_t*) args;
    while (1)
    {
        pthread_mutex_lock(&pshareRes->mutex);
        if (pshareRes->flag == 1)
        {
            pthread_mutex_unlock(&pshareRes->mutex);
            break;
        }
    }
    B();
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    shareRes_t shareRes;
    shareRes.flag = 0;
    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, &shareRes);
    A();
    pthread_mutex_lock(&shareRes.mutex);
    shareRes.flag = 1;
    pthread_mutex_unlock(&shareRes.mutex);
    pthread_join(tid, NULL);
    return 0;
}
