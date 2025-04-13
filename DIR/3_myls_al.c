#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    DIR* dirp = opendir(argv[1]);

    ERROR_CHECK(dirp, NULL, "opendir:");

    struct dirent* pdirent;

    struct stat statbuf;

    int ret = 0;
    while ((pdirent = readdir(dirp)) != NULL)
    {
        // 文件名变成路径
        char path[1024] = {0};
        sprintf(path, "%s%s%s", argv[1], "/", pdirent->d_name);
        memset(&statbuf, 0, sizeof(statbuf));
        stat(path, &statbuf);
        printf("%o %ld %d %d %ld %s %s\n",
               statbuf.st_mode,
               statbuf.st_nlink,
               statbuf.st_uid,
               statbuf.st_gid,
               statbuf.st_size,
               ctime(&statbuf.st_mtime),
               pdirent->d_name);
    }

    closedir(dirp);
    return 0;
}
