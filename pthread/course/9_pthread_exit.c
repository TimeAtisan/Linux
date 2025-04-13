#include <Myhead.h>

void* ThreadFunc(void* args)
{
    // return NULL;
    pthread_exit(NULL);
    printf("[Can you see me!]\n");
}

int main(int argc, char* argv[])
{
    pthread_t tid;
    pthread_create(&tid, NULL, ThreadFunc, NULL);
    while (1)
    {
        sleep(1);
    }

    return 0;
}
