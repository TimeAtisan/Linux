#include <Myhead.h>

typedef struct shareRes_s
{
    int ticketNum;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} shareRes_t;

void* sellWindow1(void* args)
{
    shareRes_t* pshareRes = (shareRes_t*) args;
    while (1)
    {
        pthread_mutex_lock(&pshareRes->mutex);

        if (pshareRes->ticketNum <= 10)
        {
            pthread_cond_signal(&pshareRes->cond);
        }

        if (pshareRes->ticketNum <= 0)
        {
            pthread_mutex_unlock(&pshareRes->mutex);
            break;
        }
        printf("Before window1,ticketNum = %d\n", pshareRes->ticketNum);
        --pshareRes->ticketNum;
        printf("After window1,ticketNum = %d\n", pshareRes->ticketNum);
        if (pshareRes->ticketNum <= 10)
        {
            pthread_cond_signal(&pshareRes->cond);
        }
        pthread_mutex_unlock(&pshareRes->mutex);
        // 一定要先解锁再睡眠
        sleep(1);
    }
    pthread_exit(NULL);
}

void* sellWindow2(void* args)
{
    shareRes_t* pshareRes = (shareRes_t*) args;
    while (1)
    {
        pthread_mutex_lock(&pshareRes->mutex);

        if (pshareRes->ticketNum <= 10)
        {
            pthread_cond_signal(&pshareRes->cond);
        }

        if (pshareRes->ticketNum <= 0)
        {
            pthread_mutex_unlock(&pshareRes->mutex);
            break;
        }
        printf("Before window2,ticketNum = %d\n", pshareRes->ticketNum);
        --pshareRes->ticketNum;
        printf("After window2,ticketNum = %d\n", pshareRes->ticketNum);

        pthread_mutex_unlock(&pshareRes->mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}

void addtick(shareRes_t* pshareRes, int num)
{
    pshareRes->ticketNum += num;
}

void* addticketWindow(void* args)
{
    shareRes_t* pshareRes = (shareRes_t*) args;
    pthread_mutex_lock(&pshareRes->mutex);
    if (pshareRes->ticketNum > 10)
    {
        printf("ticket is enough now!---\n");
        pthread_cond_wait(&pshareRes->cond, &pshareRes->mutex);
    }
    addtick(pshareRes, 10);
    printf("add %d ticket!\n", 10);
    pthread_mutex_unlock(&pshareRes->mutex);
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    shareRes_t shareRes;
    shareRes.ticketNum = 20;
    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_cond_init(&shareRes.cond, NULL);

    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, sellWindow1, &shareRes);
    pthread_create(&tid2, NULL, sellWindow2, &shareRes);
    pthread_create(&tid3, NULL, addticketWindow, &shareRes);

    pthread_join(tid3, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid1, NULL);
    return 0;
}
