#include <Myhead.h>

void* threadFunc(void* arg)
{
    int* pi = (int*) arg;
    printf("child:pi = %d\n", *pi);
    ++*pi;
    return NULL;
}

void func()
{
    int i = 100;
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, &i);
}

int main(int argc, const char* argv[])
{
    func();
    sleep(1);
    return 0;
}
