#include <Myhead.h>

int main(int argc, char* argv[])
{
    pthread_mutex_t mutex;

    // 定义一个锁属性类型
    pthread_mutexattr_t mutexattr;
    // 初始化锁属性
    pthread_mutexattr_init(&mutexattr);
    // 设置锁属性为检错锁
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK);

    // mutex就是一个检错类型的互斥锁
    pthread_mutex_init(&mutex, &mutexattr);
    int ret = pthread_mutex_lock(&mutex);
    THREAD_ERROR_CHECK(ret, "[first lock!]\n")
    printf("[first lock!]\n");
    ret = pthread_mutex_lock(&mutex);
    THREAD_ERROR_CHECK(ret, "[second lock!]\n")
    printf("[second lock!]\n");

    return 0;
}
