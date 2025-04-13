#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    char* p = (char*) malloc(6);
    strcpy(p, "hello");
    if (pid == 0)
    {
        puts("\n");
        puts("child:");
        fprintf(stdout, "child p = %s,p = %p\n", p, &p);
        strcpy(p, "world!");
        fprintf(stdout, "child p = %s,&p = %p\n", p, &p);
    }
    else
    {
        puts("\n");
        puts("parent:");
        fprintf(stdout, "parents p = %s,p = %p\n", p, &p);
        sleep(1);
        fprintf(stdout, "parents p = %s,p = %p\n", p, &p);
    }

    return 0; // 和栈空间的表现差不多
}
