#include <Myhead.h>

void* threadFunc(void* args)
{
    int* p = (int*) malloc(sizeof(int));
    sleep(1);
    printf("child:*p = %d\n", *p);
    return NULL;
}

int main(int argc, const char* argv[])
{
    int* p = (int*) malloc(sizeof(int));
    *p = 10000;
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, p);
    free(p);
    pthread_join(tid, NULL);

    return 0;
}
