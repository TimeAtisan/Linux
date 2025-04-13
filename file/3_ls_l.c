#include <Myhead.h>

int print_file_permissions(const struct stat* statbuf, char buf[], int len)
{
    switch (statbuf->st_mode & S_IFMT)
    {
        case S_IFBLK:
            buf[0] = 'b';
            break;
        case S_IFCHR:
            buf[0] = 'c';
            break;
        case S_IFDIR:
            buf[0] = 'd';
            break;
        case S_IFIFO:
            buf[0] = 'p';
            break;
        case S_IFLNK:
            buf[0] = 'l';
            break;
        case S_IFREG:
            buf[0] = '-';
            break;
        case S_IFSOCK:
            buf[0] = 's';
            break;
        default:
            buf[0] = '?';
            break;
    }

    // 用户权限
    buf[1] = (statbuf->st_mode & S_IRUSR) ? 'r' : '-';
    buf[2] = (statbuf->st_mode & S_IWUSR) ? 'w' : '-';
    buf[3] = (statbuf->st_mode & S_IXUSR) ? 'x' : '-';

    // 组权限
    buf[4] = (statbuf->st_mode & S_IRGRP) ? 'r' : '-';
    buf[5] = (statbuf->st_mode & S_IWGRP) ? 'w' : '-';
    buf[6] = (statbuf->st_mode & S_IXGRP) ? 'x' : '-';

    // 其他用户权限
    buf[7] = (statbuf->st_mode & S_IROTH) ? 'r' : '-';
    buf[8] = (statbuf->st_mode & S_IWOTH) ? 'w' : '-';
    buf[9] = (statbuf->st_mode & S_IXOTH) ? 'x' : '-';

    buf[10] = '\0'; // 字符串结束符
}

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    DIR* dirp = opendir(argv[1]);
    struct dirent* pdirent;
    struct stat statbuf;

    chdir(argv[1]);
    while ((pdirent = readdir(dirp)) != NULL)
    {
        stat(pdirent->d_name, &statbuf);
        char buf[11];
        memset(buf, '-', sizeof(buf));
        print_file_permissions(&statbuf, buf, sizeof(buf));
        // printf("%s\n", buf);
        struct tm* time;
        time = localtime(&statbuf.st_mtime);
        printf("%s %2ld %s %s %5ld %2d月 %2d %2d:%2d %s\n",
               buf,
               statbuf.st_nlink,
               getgrgid(statbuf.st_gid)->gr_name,
               getpwuid(statbuf.st_uid)->pw_name,
               statbuf.st_size,
               time->tm_mon,
               time->tm_yday,
               time->tm_hour,
               time->tm_mday,
               pdirent->d_name);
    }

    return 0;
}
