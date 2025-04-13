#include "../Myhead.h"

int main()
{
    int fd1[2], fd2[2];
    ERROR_CHECK(pipe(fd1), -1, "pipe error!\n");
    ERROR_CHECK(pipe(fd2), -1, "pipe error!\n");
    pid_t pid = fork();
    ERROR_CHECK(pid, -1, "fork error!\n");
    if (pid > 0)
    {
        sleep(1);
        printf("parent process started:\n");
        close(fd1[1]);
        close(fd2[0]);
        char buf[1024] = {0};
        while (1)
        {
            printf("Parent: Please enter message: ");
            fflush(stdout);
            ssize_t ret = read(STDIN_FILENO, buf, sizeof(buf) - 1);
            ERROR_CHECK(ret, -1, "read error!\n")
            buf[ret] = '\n';
            write(fd2[1], buf, ret);
            memset(buf, 0, sizeof(buf));
            ret = read(fd1[0], buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read error!\n");
            buf[ret] = '\n';
            printf("Parent received: %s", buf);
        }
        close(fd1[0]);
        close(fd2[1]);
        wait(NULL);
        exit(0);
    }
    else if (pid == 0)
    {
        printf("child process started:\n");
        sleep(1);
        close(fd1[0]);
        close(fd2[1]);
        char buf[1024];
        while (1)
        {
            memset(buf, 0, sizeof(buf));
            int ret = read(fd2[0], buf, sizeof(buf) - 1);
            ERROR_CHECK(ret, -1, "read error!\n");
            buf[ret] = '\n';
            printf("Child received: %s", buf);
            printf("Child: Please enter response: ");
            fflush(stdout);
            memset(buf, 0, sizeof(buf));
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read error!\n");
            buf[ret] = '\n';
            ret = write(fd1[1], buf, ret);
            ERROR_CHECK(ret, -1, "write error!");
        }
        close(fd1[1]);
        close(fd2[0]);
        exit(0);
    }

    return 0;
}