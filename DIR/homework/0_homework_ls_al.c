#include <Myhead.h>

int setTypemode(mode_t mode, char* str)
{
    switch (mode & S_IFMT)
    {
        case S_IFBLK:
            str[0] = 'b';
            break;
        case S_IFCHR:
            str[0] = 'c';
            break;
        case S_IFDIR:
            str[0] = 'd';
            break;
        case S_IFIFO:
            str[0] = 'p';
            break;
        case S_IFLNK:
            str[0] = 'l';
            break;
        case S_IFREG:
            str[0] = '-';
            break;
        case S_IFSOCK:
            str[0] = 's';
            break;
        default:
            str[0] = '?';
            break;
    }

    if (mode & S_IRUSR)
        str[1] = 'r';
    if (mode & S_IWUSR)
        str[2] = 'w';
    if (mode & S_IXUSR)
        str[3] = 'x';

    // 组权限
    if (mode & S_IRGRP)
        str[4] = 'r';
    if (mode & S_IWGRP)
        str[5] = 'w';
    if (mode & S_IXGRP)
        str[6] = 'x';

    // 其他用户权限
    if (mode & S_IROTH)
        str[7] = 'r';
    if (mode & S_IWOTH)
        str[8] = 'w';
    if (mode & S_IXOTH)
        str[9] = 'x';
    if (mode & S_IXOTH)
        str[9] = 'x';
    str[10] = '\0';
}

int getTmineStr(time_t mtime, char* str)
{
    char momArr[][10] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Agu", "Spe", "Oct", "Nov", "Dec"};
    struct tm* timeVal = localtime(&mtime);
    int mon = timeVal->tm_mon;
    int mday = timeVal->tm_mday;
    int hour = timeVal->tm_hour;
    int min = timeVal->tm_min;
    sprintf(str, "%s %02d %02d:%02d", momArr[mon], mday, hour, min);
    return 0;
}

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    DIR* dirp = opendir(argv[1]);
    ERROR_CHECK(dirp, NULL, "opendir");
    struct dirent* pdirent;
    int ret = chdir(argv[1]);
    ERROR_CHECK(ret, -1, "chdir");

    // readdir只返回相对目录的文件名，不包含 argv[1]（用户指定的路径）。
    while ((pdirent = readdir(dirp)) != NULL)
    {
        struct stat statbuf;
        int ret = stat(pdirent->d_name, &statbuf);
        ERROR_CHECK(ret, -1, "stat");
        char str[1024] = {0};
        memset(str, '-', sizeof(str));
        setTypemode(statbuf.st_mode, str);
        char timeBuf[1024] = {0};
        getTmineStr(statbuf.st_mtime, timeBuf);

        printf("%s %2ld %s %s %5ld %s %s\n",
               str,
               statbuf.st_nlink,
               getpwuid(statbuf.st_uid)->pw_name,
               getgrgid(statbuf.st_gid)->gr_name,
               statbuf.st_size,
               timeBuf,
               pdirent->d_name);
    }
    closedir(dirp);

    return 0;
}

/* #include <Myhead.h>

int ftype_permission(char *type_permission,mode_t mode)
{
  switch (mode & S_IFMT)
  {
    case S_IFBLK:  type_permission[0] = 'b'; break;
    case S_IFCHR:  type_permission[0] = 'c'; break;
    case S_IFDIR:  type_permission[0] = 'd'; break;
    case S_IFIFO:  type_permission[0] = 'p'; break;
    case S_IFLNK:  type_permission[0] = 'l'; break;
    case S_IFREG:  type_permission[0] = '-'; break;
    case S_IFSOCK: type_permission[0] = 's'; break;
    default:       type_permission[0] = '?'; break;
  }

  if (mode & S_IRUSR) type_permission[1] = 'r';
  if (mode & S_IWUSR) type_permission[2] = 'w';
  if (mode & S_IXUSR) type_permission[3] = 'x';

  // 组权限
  if (mode & S_IRGRP) type_permission[4] = 'r';
  if (mode & S_IWGRP) type_permission[5] = 'w';
  if (mode & S_IXGRP) type_permission[6] = 'x';

  // 其他用户权限
  if (mode & S_IROTH) type_permission[7] = 'r';
  if (mode & S_IWOTH) type_permission[8] = 'w';
  if (mode & S_IXOTH) type_permission[9] = 'x';

  return 0;
}

int main(int argc,char *argv[])
{
  ARGS_CHECK(argc,2);

  DIR *dirp = opendir(argv[1]);

  struct dirent *pdirent;
  struct stat statbuf;
  char type_permission[10];
  memset(type_permission,'-',sizeof(type_permission));
  int ret = 0;

  struct passwd *pw;
  struct group *gp;

  while ((pdirent = readdir(dirp)) != NULL)
  {
    ret = stat(pdirent->d_name,&statbuf);
    ERROR_CHECK(ret,-1,"stat:");
    ftype_permission(type_permission,statbuf.st_mode);

    pw = getpwuid(statbuf.st_uid);
    gp = getgrgid(statbuf.st_gid);

    struct tm *loc = localtime(&statbuf.st_mtime);

    printf("%s %ld %s %s %5ld %2d月 %d %d:%d %s\n",
      type_permission,
      statbuf.st_nlink,
      pw->pw_name,
      gp->gr_name,
      statbuf.st_size,
      loc->tm_mon,
      loc->tm_mday,
      loc->tm_hour,
      loc->tm_sec,
      pdirent->d_name);
  }
  return 0;
}
 */