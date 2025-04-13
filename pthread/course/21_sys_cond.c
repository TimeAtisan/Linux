#include <Myhead.h>

typedef struct shareRes_s
{
    int flag;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} shareRes_t;

void A()
{
    printf("Before: A\n");
    sleep(3);
    printf("After: A\n");
}

void B()
{
    printf("Before: B\n");
    sleep(3);
    printf("After: B\n");
}

void* threadFunc(void* args)
{
    sleep(5);
    shareRes_t* pshareRes = (shareRes_t*) args;
    pthread_mutex_lock(&pshareRes->mutex);
    if (pshareRes->flag == 0)
    {
        // 检查是否有mutex-> 移入唤醒队列 -> 解锁并等待
        pthread_cond_wait(&pshareRes->cond, &pshareRes->mutex);
        // 唤醒--> 加锁 --> 带锁返回
    }
    pthread_mutex_unlock(&pshareRes->mutex);
    B();
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

    A();
    pthread_mutex_lock(&shareRes.mutex);
    shareRes.flag = 1;
    pthread_cond_signal(&shareRes.cond);
    pthread_mutex_unlock(&shareRes.mutex);
    pthread_join(tid, NULL);

    return 0;
}
