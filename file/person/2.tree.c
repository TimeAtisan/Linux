#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define SIZE 1024

#define ARGC_CHECK(argc, num)                 \
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

int printDir(char* path)
{
    DIR* pdir = opendir(path);
    ERROR_CHECK(pdir, NULL, "opendir");
    struct dirent* pdirent;
    char buf[SIZE];

    while ((pdirent = readdir(pdir)))
    {
        if (strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
        {
            continue;
        }
        fprintf(stdout, "%s\n", pdirent->d_name);
        sprintf(buf, "%s%s%s", path, "/", pdirent->d_name);
        if (pdirent->d_type == 4)
        {
            printDir(buf);
        }
    }
    closedir(pdir);
    return 0;
}

int tabPrintDir(char* path, int width)
{
    DIR* pdir = opendir(path);
    ERROR_CHECK(pdir, NULL, "opendir");
    struct dirent* pdirent;
    char buf[SIZE];
    while ((pdirent = readdir(pdir)))
    {
        if (strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
        {
            continue;
        }
        fprintf(stdout, "%s%s%s\n", "|__", "     ", pdirent->d_name);
        sprintf(buf, "%s%s%s", path, "/", pdirent->d_name);
        if (pdirent->d_type == 4)
        {
            printf(" ");
            tabPrintDir(buf, width + 4);
        }
    }
    closedir(pdir);
    return 0;
}

int main(int argc, char* argv[])
{
    ARGC_CHECK(argc, 2);
    puts(argv[1]);
    // printDir(argv[1]);
    tabPrintDir(argv[1], 0);
    return 0;
}