#include <Myhead.h>

void* threadFunc(void* args)
{
    while (1)
    {
        // printf("I still alive!\n");
        pthread_testcancel();
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    pthread_t tid;

    pthread_create(&tid, NULL, threadFunc, NULL);
    sleep(1);

    printf("send cancellation request\n");
    pthread_cancel(tid);

    void* retval;
    pthread_join(tid, &retval);
    printf("retval = %ld\n", (long) retval);
    return 0;
}
