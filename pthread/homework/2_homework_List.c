#include <Myhead.h>

typedef struct _Node
{
    int val;
    struct _Node* next;
} Node;

typedef struct _List
{
    Node* head;

} List;

List* initList()
{
    List* list = malloc(sizeof(List));
    list->head = malloc(sizeof(Node));
    list->head->next = NULL;
    list->head->val = 0;
    return list;
}

void insert(List** linklist, int val)
{
    List* list = *linklist;
    Node* node = malloc(sizeof(Node));
    node->val = val;
    node->next = NULL;
    Node* tmp = list->head->next;
    list->head->next = node;
    node->next = tmp;
}

void* threadFunc(void* args)
{
    List* list = (List*) args;
    Node* tmp = list->head->next;

    while (tmp != NULL)
    {
        printf("%d ", tmp->val);
        tmp = tmp->next;
    }
    puts("");
}

void ListFree(List** linklist)
{
    List* list = *linklist;
    Node* node = list->head;
    while (node != NULL)
    {
        Node* tmp = node->next;
        free(node);
        node = tmp;
    }
    free(list);
    *linklist = NULL;
}

int main(int argc, const char* argv[])
{
    List* list = initList();
    insert(&list, 1);
    insert(&list, 2);
    insert(&list, 3);
    insert(&list, 4);
    insert(&list, 5);
    insert(&list, 6);

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, threadFunc, list);
    THREAD_ERROR_CHECK(ret, "thread_create");
    pthread_join(tid, NULL);
    ListFree(&list);
    return 0;
}
