#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define ARGS_CHECK(argc, num)                        \
    {                                                \
        if (num != argc)                             \
        {                                            \
            fprintf(stdout, "%s", "args is error!"); \
            return -1;                               \
        }                                            \
    }

#define ERROR_CHECK(ret, num, msg) \
    {                              \
        if (ret == num)            \
        {                          \
            perror(msg);           \
            return -1;             \
        }                          \
    }

int main()
{
    pid_t pid = fork();
    int i = 0;
    if (pid == 0)
    {
        puts("\n");
        puts("child:");
        fprintf(stdout, "child i = %d,&i = %p\n", i, &i);
        ++i;
        fprintf(stdout, "child i = %d,&i = %p\n", i, &i);
    }
    else
    {
        puts("\n");
        puts("parent:");
        fprintf(stdout, "parents i = %d,&i = %p\n", i, &i);
        --i;
        fprintf(stdout, "parents i = %d,&i = %p\n", i, &i); // 子进程会拷贝父进程的内容，但是修改的内容会互相独立
    }

    return 0;
}
// 父子进程中的变量包括其地址是一致