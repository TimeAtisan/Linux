#include <Myhead.h>
#define NUM 10000000

void* threadFunc(void* args)
{
    int* pVal = (int*) args;
    for (size_t i = 0; i < NUM; i++)
    {
        ++(*pVal);
    }
    pthread_exit(NULL);
}
int main(int argc, char* argv[])
{
    pthread_t tid;
    int val = 0;
    pthread_create(&tid, NULL, threadFunc, &val);

    for (size_t i = 0; i < NUM; i++)
    {
        ++val;
    }

    pthread_join(tid, NULL);
    printf("[val = %d]\n", val);

    return 0;
}
