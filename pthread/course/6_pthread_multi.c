#include <Myhead.h>

typedef struct _threadID
{
    pthread_t tid;
    int i;
} threadID;

void* threadFunc(void* args)
{
    threadID* pi = (threadID*) args;
    printf("chid%ld:i = %d\n", pi->tid, pi->i);
    ++pi->i;
    return NULL;
}
int main(int argc, const char* argv[])
{
    threadID* Tid;
    Tid = malloc(sizeof(threadID));
    Tid->i = 1000;
    for (int i = 0; i < 3; ++i)
    {
        pthread_create(&Tid->tid, NULL, threadFunc, Tid);
    }
    printf("main:i = %d\n", Tid->i);

    return 0;
}
