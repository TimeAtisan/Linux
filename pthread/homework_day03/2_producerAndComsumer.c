#include <Myhead.h>

typedef struct node_s
{
    int data;
    struct node_s* pNext;
} node_t;

typedef struct queue_s
{
    node_t* pFront;
    node_t* pRear;
    int queueSize;
} queue_t;

int enQueue(queue_t* pQueue, int data)
{
    if (pQueue == NULL)
    {
        printf("queue is NULL!\n");
        return -1;
    }
    node_t* pNew = (node_t*) malloc(sizeof(node_t));
    pNew->data = data;
    pNew->pNext = NULL;

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
        printf("Queue has no node!\n");
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

/* int initQueue(queue_t *pQueue)
{
  pQueue->pFront = NULL;
  pQueue->pRear = NULL;
  pQueue->queueSize = 0;
  return 0;
} */

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
        printf("producer,current quesize = %d\n", pshareRes->queue.queueSize);
        printf("[tid = %lu\n]", pthread_self());

        int data = 100 + rand() % 900;
        enQueue(&pshareRes->queue, data);
        printf("producer: data = %d\n", data);
        visitQueue(&pshareRes->queue);
        pthread_cond_broadcast(&pshareRes->cond);
        printf("------------------------------\n");
        pthread_mutex_unlock(&pshareRes->mutex);
        sleep(3);
    }

    pthread_exit(NULL);
}

void* consumer(void* args)
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
        printf("producer,current quesize = %d\n", pshareRes->queue.queueSize);
        printf("[tid = %lu\n]", pthread_self());
        int data = pshareRes->queue.pFront->data;
        printf("consumer: front = %d\n", data);
        deQueue(&pshareRes->queue);
        visitQueue(&pshareRes->queue);
        pthread_cond_broadcast(&pshareRes->cond);
        printf("------------------------------\n");
        pthread_mutex_unlock(&pshareRes->mutex);
        sleep(1);
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    shareRes_t shareRes;
    memset(&shareRes, 0, sizeof(shareRes));

    for (size_t i = 0; i < 8; i++)
    {
        sleep(1);
        int data = 100 + rand() % 900;
        printf("data = %d\n", data);
        enQueue(&shareRes.queue, data);
        visitQueue(&shareRes.queue);
    }
    printf("------------------------------\n");
    printf("------------------------------\n");

    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_cond_init(&shareRes.cond, NULL);
    pthread_t tid[5];
    int i = 0;

    for (; i < 3; i++)
    {
        pthread_create(&tid[i], NULL, producer, &shareRes);
    }

    for (; i < 5; i++)
    {
        pthread_create(&tid[i], NULL, consumer, &shareRes);
    }

    for (size_t i = 0; i < 8; i++)
    {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&shareRes.mutex);
    pthread_cond_destroy(&shareRes.cond);

    return 0;
}

/* int main(int argc,char *argv[])
{
  queue_t queue;
  queue.pFront = NULL;
  queue.pRear = NULL;
  queue.queueSize = 0;
  for (size_t i = 0; i < 10; i++)
  {
    int data = rand()%1000;
    printf("data = %d\n",data);
    enQueue(&queue,data);
    sleep(1);
    visitQueue(&queue);
  }
  printf("-----------------\n");
  for (size_t i = 0; i < 10; i++)
  {
    int data = queue.pFront->data;
    printf("front = %d\n",data);
    deQueue(&queue);
    sleep(1);
    visitQueue(&queue);
  }
  return 0;
} */
