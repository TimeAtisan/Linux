#include <Myhead.h>
#define NUM 100000000

typedef struct shareRes_s
{
    int val;
    pthread_mutex_t mutex;
} shareRes_t;

void* threadFunc(void* args)
{
    shareRes_t* pshareRes = (shareRes_t*) args;
    for (size_t i = 0; i < NUM; i++)
    {
        pthread_mutex_lock(&pshareRes->mutex);
        ++pshareRes->val;
        pthread_mutex_unlock(&pshareRes->mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    shareRes_t shareRes;
    shareRes.val = 0;
    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_t tid;

    struct timeval begTime, endTime;

    gettimeofday(&begTime, NULL);
    pthread_create(&tid, NULL, threadFunc, &shareRes);

    for (size_t i = 0; i < NUM; i++)
    {
        pthread_mutex_lock(&shareRes.mutex);
        ++shareRes.val;
        pthread_mutex_unlock(&shareRes.mutex);
    }

    pthread_join(tid, NULL);
    gettimeofday(&endTime, NULL);
    printf("[val = %d,total time = %ld]\n",
           shareRes.val,
           (endTime.tv_sec - begTime.tv_sec) * 1000 * 1000 + (endTime.tv_usec - begTime.tv_usec));

    return 0;
}
