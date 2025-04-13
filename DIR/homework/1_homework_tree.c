#include <Myhead.h>

// 至少有一个路径的参数
int DFSprint(char* dirpath, int width)
{
    DIR* dirp = opendir(dirpath);
    ERROR_CHECK(dirp, NULL, "opendir");
    struct dirent* pdirent;
    while ((pdirent = readdir(dirp)) != NULL)
    {
        if (strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
        {
            continue;
        }

        printf("|");
        for (int i = 0; i < width; ++i)
        {
            printf("--");
        }
        printf("%s\n", pdirent->d_name);
        if (pdirent->d_type == DT_DIR)
        {
            char path[1024] = {0};
            sprintf(path, "%s%s%s", dirpath, "/", pdirent->d_name);
            DFSprint(path, width + 2);
        }
    }
    return 0;
}

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    printf("%s\n", argv[1]);
    DFSprint(argv[1], 0);
    return 0;
}
