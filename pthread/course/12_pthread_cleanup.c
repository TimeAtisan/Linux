#include <Myhead.h>

void freep1(void* arg)
{
    printf("free p1\n");
    free((int*) arg);
}

void freep2(void* arg)
{
    printf("free p2\n");
    free((int*) arg);
}

void closefd3(void* arg)
{
    int* pfd = (int*) arg;
    printf("close fd3!\n");
    close(*pfd);
}

void* threadFunc(void* arg)
{
    // 申请资源后，马上压栈
    // 把释放行为替换成弹栈
    int* p1 = (int*) malloc(sizeof(int));
    // 两者之间什么都不能做
    pthread_cleanup_push(freep1, p1);

    int* p2 = (int*) malloc(sizeof(int));
    pthread_cleanup_push(freep2, p2);

    int fd3 = open("file", O_RDONLY);
    pthread_cleanup_push(closefd3, &fd3);

    // ...
    // closefd3(&fd3);
    pthread_cleanup_pop(1); // 参数填正数

    printf("--------------------------\n");
    pthread_exit(NULL);
    // free(p2);
    // freep2(p2);
    pthread_cleanup_pop(1); // 参数填正数
    // free(p1);
    // freep1(p1);
    // freep1(p1);
    pthread_cleanup_pop(1); // 参数填正数
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, NULL);
    pthread_join(tid, NULL);
    return 0;
}
