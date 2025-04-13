#include <Myhead.h>

typedef struct shareRes_s
{
    int flag;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} shareRes_t;

void* threadFunc1(void* args)
{
    shareRes_t* pshareRes = (shareRes_t*) args;
    while (1)
    {
        pthread_mutex_lock(&pshareRes->mutex);
        if (pshareRes->flag <= 0)
        {
            printf("All ticket has been done!\n");
            pthread_mutex_unlock(&pshareRes->mutex);
            break;
        }
        printf("before buy window1,ticketNumber = %d\n", pshareRes->flag);
        --pshareRes->flag;
        printf("After buy window1,ticketNumber = %d\n\n", pshareRes->flag);
        if (pshareRes->flag <= 10)
        {
            // printf("----------B------------\n");
            printf("\nticket is not enough!\n\n");
            pthread_cond_signal(&pshareRes->cond);
        }
        // printf("-----------AA-----------\n");
        pthread_mutex_unlock(&pshareRes->mutex);

        sleep(2);
    }

    pthread_exit(NULL);
}

void* threadFunc2(void* args)
{
    shareRes_t* pshareRes = (shareRes_t*) args;
    while (1)
    {
        pthread_mutex_lock(&pshareRes->mutex);
        if (pshareRes->flag <= 0)
        {
            printf("All ticket has been done!\n");
            pthread_mutex_unlock(&pshareRes->mutex);
            break;
        }
        printf("before buy window2,ticketNumber = %d\n", pshareRes->flag);
        --pshareRes->flag;
        printf("After buy window2,ticketNumber = %d\n\n", pshareRes->flag);

        if (pshareRes->flag <= 10)
        {
            // printf("----------A------------\n");
            printf("\nticket is not enough!\n\n");
            pthread_cond_signal(&pshareRes->cond);
        }
        // printf("-----------BB-----------\n");

        pthread_mutex_unlock(&pshareRes->mutex);
        sleep(2);
    }

    pthread_exit(NULL);
}

/* void *addticket(void *args)
{
    shareRes_t *pshareRes = (shareRes_t *)args;

    pthread_mutex_lock(&pshareRes->mutex);
    if (pshareRes->flag > 10)
    {
        pthread_cond_wait(&pshareRes->cond, &pshareRes->mutex);
    }
    pshareRes->flag += 10;
    printf("add 10 ticket! ticketNum = %d\n", pshareRes->flag);

    pthread_mutex_unlock(&pshareRes->mutex);
    pthread_exit(NULL);
} */

int main(int argc, char* argv[])
{
    shareRes_t shareRes;
    shareRes.flag = 20;
    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_cond_init(&shareRes.cond, NULL);

    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, threadFunc1, &shareRes);
    pthread_create(&tid2, NULL, threadFunc2, &shareRes);

    pthread_mutex_lock(&shareRes.mutex);
    if (shareRes.flag > 10)
    {
        pthread_cond_wait(&shareRes.cond, &shareRes.mutex);
    }
    shareRes.flag += 10;
    printf("add 10 ticket! ticketNum = %d\n", shareRes.flag);

    pthread_mutex_unlock(&shareRes.mutex);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_mutex_destroy(&shareRes.mutex);
    pthread_cond_destroy(&shareRes.cond);
    return 0;
}
