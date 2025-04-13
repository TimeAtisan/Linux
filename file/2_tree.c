#include <Myhead.h>

int tree(const char* path, int width)
{
    DIR* dirp = opendir(path);
    ERROR_CHECK(dirp, NULL, "opendir");
    struct dirent* pdirent;
    char buf[1024] = {0};

    while (pdirent = readdir(dirp))
    {
        if (strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
        {
            continue;
        }
        for (size_t i = 0; i < width; i++)
        {
            printf(" ");
        }

        printf("%s\n", pdirent->d_name);

        if (pdirent->d_type == 4)
        {
            sprintf(buf, "%s%s%s", path, "/", pdirent->d_name);
            tree(buf, 4);
        }
    }
}

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    puts(argv[1]);
    tree(argv[1], 0);

    return 0;
}
