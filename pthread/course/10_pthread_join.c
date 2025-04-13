#include <Myhead.h>

void* threadFunc(void* args)
{
    // 退出方法一：
    // pthread_exit(NULL); // 参数为NULL则把0传给指针

    // 退出方法二：
    pthread_exit((void*) 123);
    /*
      retval = 123
     */

    // 退出方法三：
    // 不推荐此方法，不符合谁开发谁治理原则
    // int *p = (int*)malloc(sizeof(int));
    // pthread_exit(p);
}

int main(int argc, char* argv[])
{
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, NULL);
    void* retval;
    pthread_join(tid, &retval); // 成立，尽管retval的指向没有分配内存，但是retval分配了内存在栈上

    /* void **retval;
    pthread_join(tid,retval); // 不成立，因为没有分配内存，会犯野指针错误
    但是可以使用
    void **retval = (void**)malloc(sizeof(void*));
    pthread_join(tid,retval);
    */

    printf("retval = %ld\n", (long) retval);
    return 0;
}
