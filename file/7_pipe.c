#include <Myhead.h>

int main(int argc, char* argv[])
{
    int fd[2];
    pipe(fd);
    for (size_t i = 0; i < 3; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            close(fd[1]);
            while (1)
            {
                char buf[4096] = {0};
                int ret = read(fd[0], buf, sizeof(buf));
                ERROR_CHECK(ret, -1, "read");
                if (ret == 0)
                {
                    break;
                }

                fprintf(stdout, "%s\n", buf);
            }
        }
    }
    close(fd[0]);
    write(fd[1], "hello YH!", sizeof("hello YH!"));
    for (size_t i = 0; i < 3; i++)
    {
        wait(NULL);
    }

    return 0;
}
