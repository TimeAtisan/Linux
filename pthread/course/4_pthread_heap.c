#include <Myhead.h>

void* threadFunc(void* arg)
{
    int* p = (int*) arg;
    printf("child:*p = %d\n", *p);
    ++*p; // 此处若为*p++则会出错（因为++ 的优先级高于*）
    return NULL;
}

int main(int argc, const char* argv[])
{
    int* p = (int*) malloc(sizeof(int));
    *p = 1000;
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, p);
    sleep(1);
    printf("main:*p = %d\n", *p);
    return 0;
}
