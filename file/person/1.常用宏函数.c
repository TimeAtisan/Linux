#include <stdio.h>
#include <string.h>

#define ARGS_CHECK(argc, num)                 \
    {                                         \
        if (argc != num)                      \
        {                                     \
            fprintf(stdout, "args error!\n"); \
            return -1;                        \
        }                                     \
    }

#define ERROR_CHECK(ret, num, msg) \
    {                              \
        if (ret == num)            \
        {                          \
            perror(msg);           \
            return -1;             \
        }                          \
    }

#define SIZE 20

int readAll(FILE* fp)
{
    char buf[SIZE];
    char* ret = fgets(buf, SIZE, fp);
    if (ret != NULL)
    {
        fprintf(stdout, "\n%s", buf);
        while (strchr(buf, '\n') == NULL)
        {
            ret = fgets(buf, SIZE, fp);
            if (ret == NULL)
            {
                perror("fgets error!");
                return -1;
            }
            fprintf(stdout, "%s", buf);
        }
    }
    else
    {
        perror("fgets error!");
    }
    return 0;
}

int test()
{
    FILE* fp = fopen("file1", "r+");
    if (fp == NULL)
    {
        perror("file open error!");
        return 0;
    }

    char buf[SIZE] = {0};
    fread(buf, 1, SIZE, fp);
    fprintf(stdout, "buf = %s\n", buf);

    rewind(fp);
    int i;
    fscanf(fp, "%d", &i);
    fprintf(stdout, "i = %d\n", i);
    return 0;
}

int main()
{
    // readAll(stdin);
    test();
    return 0;
}