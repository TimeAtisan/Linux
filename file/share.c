#include <Myhead.h>

typedef struct shareRes_s
{
    int flag;
    pthread_mutex_t mutex;
} shareRes_t;

int A()
{
    printf("---------A---------\n");
    return 0;
}

int B()
{
    printf("---------B---------\n");
    return 0;
}

void* threadFunc(void* args)
{
    shareRes_t* pshareRes = (shareRes_t*) args;
    while (1)
    {
        pthread_mutex_lock(&pshareRes->mutex);
        if (pshareRes->flag == 1)
        {
            B();
            pshareRes->flag = 0;
            pthread_mutex_unlock(&pshareRes->mutex);
            sleep(2);
        }
        pthread_mutex_unlock(&pshareRes->mutex);
    }
}
int main(int argc, char* argv[])
{
    shareRes_t shareRes;
    shareRes.flag = 0;
    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_t pid;
    pthread_create(&pid, NULL, threadFunc, &shareRes);
    while (1)
    {
        if (shareRes.flag == 0)
        {
            A();
            sleep(2);
            pthread_mutex_lock(&shareRes.mutex);
            shareRes.flag = 1;
            pthread_mutex_unlock(&shareRes.mutex);
        }
    }
    pthread_mutex_destroy(&shareRes.mutex);
    return 0;
}
