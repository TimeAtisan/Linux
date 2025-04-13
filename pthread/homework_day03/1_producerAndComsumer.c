#include <Myhead.h>

typedef struct node_s
{
    int data;
    struct node_s* pNext;
} node_t;

typedef struct queue_s
{
    int queueSize;
    node_t* pFront;
    node_t* pRear;
} queue_t;

int enQueue(queue_t* pQueue, int data)
{
    node_t* pNew = (node_t*) calloc(1, sizeof(node_t));
    pNew->data = data;
    if (pQueue->queueSize == 0)
    {
        pQueue->pFront = pNew;
        pQueue->pRear = pNew;
    }
    else
    {
        pQueue->pRear->pNext = pNew;
        pQueue->pRear = pNew;
    }
    ++pQueue->queueSize;
    return 0;
}

int deQueue(queue_t* pQueue)
{
    if (pQueue->queueSize == 0)
    {
        fprintf(stderr, "[Queue is empty!]\n");
        return -1;
    }
    node_t* pCur = pQueue->pFront;
    pQueue->pFront = pCur->pNext;
    if (pQueue->queueSize == 1)
    {
        pQueue->pRear = NULL;
    }
    free(pCur);
    --pQueue->queueSize;

    return 0;
}

int visitQueue(queue_t const* pQueue)
{
    node_t* pCur = pQueue->pFront;
    while (pCur)
    {
        printf("%d ", pCur->data);
        pCur = pCur->pNext;
    }
    printf("\n");
    return 0;
}

typedef struct shareRes_s
{
    queue_t queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

} shareRes_t;

void* producer(void* args)
{
    shareRes_t* pshareRes = (shareRes_t*) args;
    while (1)
    {
        pthread_mutex_lock(&pshareRes->mutex);
        while (pshareRes->queue.queueSize >= 10)
        {
            pthread_cond_wait(&pshareRes->cond, &pshareRes->mutex);
        }
        printf("producer,current queuesize = %d\n", pshareRes->queue.queueSize);
        printf("[tid = %lu\n]", pthread_self());
        int data = rand() % 1000;
        enQueue(&pshareRes->queue, data);
        printf("data = %d\n", data);
        visitQueue(&pshareRes->queue);
        printf("after producer,cuerrent queuesize = %d\n", pshareRes->queue.queueSize);
        pthread_cond_broadcast(&pshareRes->cond);
        pthread_mutex_unlock(&pshareRes->mutex);
        sleep(3); // 一定先解锁再睡觉，否则其他线程无法使用共享资源
    }
}

void* comsumer(void* args)
{
    shareRes_t* pshareRes = (shareRes_t*) args;
    sleep(5);
    while (1)
    {
        pthread_mutex_lock(&pshareRes->mutex);
        while (pshareRes->queue.queueSize <= 0)
        {
            pthread_cond_wait(&pshareRes->cond, &pshareRes->mutex);
        }
        printf("Before consummer,current queuesize = %d\n", pshareRes->queue.queueSize);
        printf("[tid = %lu\n]", pthread_self());
        int front = pshareRes->queue.pFront->data;
        deQueue(&pshareRes->queue);
        printf("front = %d\n", front);
        visitQueue(&pshareRes->queue);
        printf("after consumer,cuerrent queuesize = %d\n", pshareRes->queue.queueSize);
        pthread_cond_broadcast(&pshareRes->cond);
        pthread_mutex_unlock(&pshareRes->mutex);
        sleep(1); // 一定先解锁再睡觉，否则其他线程无法使用共享资源
    }
}

int main(int argc, char* argv[])
{
    shareRes_t shareRes;
    memset(&shareRes.queue, 0, sizeof(shareRes.queue));
    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_cond_init(&shareRes.cond, NULL);

    // 先插入8个元素
    for (size_t i = 0; i < 8; i++)
    {
        int data = rand() % 1000;
        enQueue(&shareRes.queue, data);
        printf("data = %d\n", data);
        visitQueue(&shareRes.queue);
    }
    printf("-------------------------------\n");

    pthread_t tid[5];
    int i = 0;
    for (; i < 3; i++)
    {
        pthread_create(&tid[i], NULL, producer, &shareRes);
    }

    for (; i < 5; i++)
    {
        pthread_create(&tid[i], NULL, comsumer, &shareRes);
    }

    for (i = 0; i < 5; i++)
    {
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&shareRes.mutex);
    pthread_cond_destroy(&shareRes.cond);

    return 0;
}

/* int main(int argc,char *argv[])
{
  queue_t pQueue;
  memset(&pQueue,0,sizeof(queue_t));
  for (size_t i = 0; i < 10; i++)
  {
    int data = rand()%1000;
    enQueue(&pQueue,data);
    printf("data = %d\n",data);
    visitQueue(&pQueue);
  }
  printf("-------------------------\n");
  for (size_t i = 0; i < 10; i++)
  {
    int front = pQueue.pFront->data;
    printf("front = %d\n",front);
    deQueue(&pQueue);
    visitQueue(&pQueue);
  }


  return 0;
} */
