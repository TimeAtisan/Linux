#include <Myhead.h>
pthread_mutex_t mutex;

void* threadFunc(void* args)
{
    pthread_mutex_lock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    // 定义一个锁属性类型
    pthread_mutexattr_t mutexattr;
    // 初始化锁属性
    pthread_mutexattr_init(&mutexattr);
    // 设置锁属性为检错锁
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK);
    // mutex就是一个检错类型的互斥锁
    pthread_mutex_init(&mutex, &mutexattr);

    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, NULL);
    sleep(1);
    int ret = pthread_mutex_lock(&mutex);
    THREAD_ERROR_CHECK(ret, "lock!\n");
    printf("[first lock!]\n");
    pthread_mutex_lock(&mutex);

    return 0;
}
