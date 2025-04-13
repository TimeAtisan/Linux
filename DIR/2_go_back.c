#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    DIR* dirp = opendir(argv[1]);

    ERROR_CHECK(dirp, NULL, "opendir:");

    struct dirent* pdirent;

    long loc;

    while ((pdirent = readdir(dirp)) != NULL)
    {
        printf("inode = %ld, reclen = %d, type = %d,name = %s\n",
               pdirent->d_ino,
               pdirent->d_reclen,
               pdirent->d_type,
               pdirent->d_name);

        if (strcmp("..", pdirent->d_name) == 0)
        {
            loc = telldir(dirp);
        }
    }
    printf("\n-------------------------------------\n\n");
    seekdir(dirp, loc);
    pdirent = readdir(dirp);
    printf("inode = %ld, reclen = %d, type = %d,name = %s\n",
           pdirent->d_ino,
           pdirent->d_reclen,
           pdirent->d_type,
           pdirent->d_name);

    closedir(dirp);
    return 0;
}
