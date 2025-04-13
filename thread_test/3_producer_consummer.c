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
    node_t* pNew = (node_t*) malloc(sizeof(node_t));
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
        printf("[the Queue is Empty!]\n");
        return -1;
    }

    node_t* pCur = pQueue->pFront;
    pQueue->pFront = pCur->pNext;

    if (pQueue->queueSize == 1)
    {
        pQueue->pRear = NULL;
    }
    --pQueue->queueSize;
    free(pCur);

    return 0;
}

int visitQueue(queue_t* pQueue)
{
    node_t* pCur = pQueue->pFront;
    while (pCur)
    {
        printf("%d ", pCur->data);
        pCur = pCur->pNext;
    }
    puts("");
    return 0;
}
typedef struct shareRes_s
{
    queue_t queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

} shareRes_t;

/* int main(int argc, char *argv[])
{
    queue_t queue;
    bzero(&queue, sizeof(queue));
    for (size_t i = 0; i < 10; i++)
    {
        int data = rand() % 1000;
        printf("enQueue data = %d\n", data);
        enQueue(&queue, data);
        sleep(1);
        visitQueue(&queue);
    }

    printf("\n---------------------------------------------\n");

    for (size_t i = 0; i < 10; i++)
    {
        int data = queue.pFront->data;
        printf("deQueue data = %d\n", data);
        deQueue(&queue);
        sleep(1);
        visitQueue(&queue);
    }

    return 0;
} */

void* producer(void* args)
{
}
void* comsumer(void* args)
{
}

int main(int argc, char* argv[])
{
    shareRes_t shareRes;
    memset(&shareRes, 0, sizeof(shareRes));
    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_cond_init(&shareRes.cond, NULL);

    for (size_t i = 0; i < 8; i++)
    {
        int data = rand() % 1000;
        enQueue(&shareRes.queue, data);
        printf("data = %d\n", data);
        visitQueue(&shareRes.queue);
    }

    pthread_t tid[5];
    size_t i = 0;
    for (; i < 3; i++)
    {
        pthread_create(&tid[i], NULL, producer, &shareRes);
    }

    for (; i < 5; i++)
    {
        /* code */
    }

    return 0;
}