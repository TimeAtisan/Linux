#include <Myhead.h>
int DFSprint(char* dirpath, int width)
{
    DIR* dirp = opendir(dirpath);
    ERROR_CHECK(dirp, NULL, "opendir");

    struct dirent* pdirent;
    while ((pdirent = readdir(dirp)) != NULL)
    {
        if (strcmp(pdirent->d_name, "..") == 0 || strcmp(pdirent->d_name, ".") == 0)
        {
            continue;
        }

        if (pdirent->d_type == DT_DIR)
        {
            printf("|");
            char path[1024];
            sprintf(path, "%s%s%s", dirpath, "/", pdirent->d_name);
            DFSprint(path, width + 2);
        }
        for (size_t i = 0; i < width; i++)
        {
            printf("-");
        }
        printf("%s\n", pdirent->d_name);
    }

    return 0;
}
int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    printf("%s\n", argv[1]);
    DFSprint(argv[1], 2);
    return 0;
}
