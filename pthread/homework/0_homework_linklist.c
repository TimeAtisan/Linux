#include <Myhead.h>

typedef struct node_s
{
    int data;
    struct node_s* pNext;
} node_t;

void headInsert(node_t** ppHead, node_t** ppTail, int data)
{
    node_t* pNew = (node_t*) malloc(sizeof(node_t));

    memset(pNew, 0, sizeof(node_t));
    pNew->data = data;
    if (*ppHead == NULL)
    {
        *ppHead = pNew;
        *ppTail = pNew;
    }
    else
    {
        pNew->pNext = *ppHead;
        *ppHead = pNew;
    }
}

void tailInsert(node_t** ppHead, node_t** ppTail, int data)
{
    node_t* pNew = (node_t*) calloc(1, sizeof(node_t));
    pNew->data = data;
    if (*ppHead == NULL && *ppTail == NULL)
    {
        *ppHead = pNew;
        *ppTail = pNew;
    }
    else
    {
        (*ppTail)->pNext = pNew;
        (*ppTail) = pNew;
        (*ppTail) = pNew;
    }
}

void visitList(void* args)
{
    node_t* pCur = (node_t*) args;
    while (pCur)
    {
        printf("%d ", pCur->data);

        pCur = pCur->pNext;
    }
    puts("");
}

/* int main(int argc,const char *argv[])
{
  node_t *pHead = NULL;
  node_t *pTail = NULL;
  int i = 0;
  for (size_t i = 0; i < 10; i++)
  {
    // HeadInsert(&pHead,&pTail,i);
    TailInsert(&pHead,&pTail,i);
    visitList(pHead);
  }
  return 0;
} */
typedef struct list_s
{
    node_t* pHead;
    node_t* pTail;
} list_t;
void* threadFunc(void* args)
{
    list_t* pList = (list_t*) args;
    printf("I am child!\n");
    visitList(pList->pHead);
    return NULL;
}

int main(int argc, const char* argv[])
{
    list_t list;
    list.pHead = NULL;
    list.pTail = NULL;
    int i = 0;
    for (size_t i = 0; i < 10; i++)
    {
        tailInsert(&list.pHead, &list.pTail, i);
    }
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, &list);
    pthread_join(tid, NULL);
    return 0;
}