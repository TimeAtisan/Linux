#include <Myhead.h>

int main(int argc, char* argv[])
{
    // 解法一：
    // ./homework_b dir1
    /*
    ARGS_CHECK(argc,2);
    DIR* dirp = opendir(argv[1]);
    ERROR_CHECK(dirp,NULL,"opendir error!\n");
    struct dirent *pdirent;
    while ((pdirent = readdir(dirp))!=NULL)
    {
      printf("%s\n",pdirent->d_name);
    }

    closedir(dirp);
    */

    // 解法二：

    FILE* fp = popen(argv[1], "r");
    ERROR_CHECK(fp, NULL, "popen error!\n");
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    int ret = fread(buf, 1, sizeof(buf), fp);
    ERROR_CHECK(ret, -1, "read error!\n");
    fprintf(stdout, "%s\n", buf);

    return 0;
}