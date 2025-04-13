#include <Myhead.h>

typedef struct shareRes_s
{
    int flag;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} shareRes_t;

void A()
{
    printf("A\n");
}

void B()
{
    printf("B\n");
}

void C()
{
    printf("C\n");
}

void* threadFunc1(void* args)
{
    shareRes_t* pshareRes = (shareRes_t*) args;
    sleep(1);
    A();
    sleep(1);

    pthread_mutex_lock(&pshareRes->mutex);
    pshareRes->flag = 1;
    pthread_mutex_unlock(&pshareRes->mutex);
    pthread_cond_signal(&pshareRes->cond);

    // printf("-----2---------\n");

    pthread_mutex_lock(&pshareRes->mutex);
    while (pshareRes->flag != 2)
    {
        pthread_cond_wait(&pshareRes->cond, &pshareRes->mutex);
    }
    pthread_mutex_unlock(&pshareRes->mutex);
    C();
    // pthread_mutex_lock(&pshareRes->mutex);
    // pshareRes->flag = 0;
    // pthread_mutex_unlock(&pshareRes->mutex);
    pthread_exit(NULL);
}

void* threadFunc2(void* args)
{
    shareRes_t* pshareRes = (shareRes_t*) args;
    pthread_mutex_lock(&pshareRes->mutex);
    // printf("-----3---------\n");

    while (pshareRes->flag != 1)
    {
        pthread_cond_wait(&pshareRes->cond, &pshareRes->mutex);
    }
    pthread_mutex_unlock(&pshareRes->mutex);
    // printf("-----4---------\n");

    B();
    sleep(1);
    pthread_mutex_lock(&pshareRes->mutex);
    pshareRes->flag = 2;
    pthread_mutex_unlock(&pshareRes->mutex);

    pthread_cond_signal(&pshareRes->cond);

    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    shareRes_t shareRes;
    shareRes.flag = 0;
    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_cond_init(&shareRes.cond, NULL);

    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, threadFunc1, &shareRes);
    // printf("-----1---------\n");
    pthread_create(&tid2, NULL, threadFunc2, &shareRes);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_cond_destroy(&shareRes.cond);
    pthread_mutex_destroy(&shareRes.mutex);
    return 0;
}
