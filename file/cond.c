#include <Myhead.h>

typedef struct shareRes_s
{
    int flag;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} shareRes_t;

int A()
{
    printf("---------A----------\n");
    return 0;
}

int B()
{
    printf("---------B----------\n");
    return 0;
}

void* threadFunc(void* args)
{
    shareRes_t* pshareRes = (shareRes_t*) args;
    while (1)
    {
        pthread_mutex_lock(&pshareRes->mutex);
        if (pshareRes->flag != 1)
        {
            pthread_cond_wait(&pshareRes->cond, &pshareRes->mutex);
        }
        B();
        pshareRes->flag = 0;
        pthread_mutex_unlock(&pshareRes->mutex);
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    shareRes_t shareRes;
    shareRes.flag = 0;
    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_cond_init(&shareRes.cond, NULL);
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, &shareRes);
    while (1)
    {
        if (shareRes.flag == 0)
        {
            A();
            pthread_mutex_lock(&shareRes.mutex);
            shareRes.flag = 1;
            pthread_cond_signal(&shareRes.cond);
            pthread_mutex_unlock(&shareRes.mutex);
            sleep(2);
        }
    }

    pthread_join(tid, NULL);
    pthread_cond_destroy(&shareRes.cond);
    pthread_mutex_destroy(&shareRes.mutex);

    return 0;
}
