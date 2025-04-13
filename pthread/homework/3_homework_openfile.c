#include <Myhead.h>

void* threadFunc(void* args)
{
    int* pfd = (int*) args;
    char buf[1024] = {0};
    lseek(*pfd, 0, SEEK_SET);
    read(*pfd, buf, sizeof(buf));
    printf("buf = %s\n", buf);
    return NULL;
}

int main(int argc, char* argv[])
{
    // 共享文件描述符和偏移量
    // ./3_homework_openfile file
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open error!\n");

    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, &fd);
    write(fd, "hello YH!", 9);

    pthread_join(tid, NULL);

    return 0;
}
