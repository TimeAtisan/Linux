#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE* fp;

    // 打开一个子进程，运行 "grep pattern" 命令
    fp = popen("grep pattern", "w");
    if (fp == NULL)
    {
        perror("popen failed");
        return EXIT_FAILURE;
    }

    // 向子进程的输入流写入数据
    fprintf(fp, "This is a test line with pattern\n");
    fprintf(fp, "Another line without the pattern\n");

    // 关闭文件流，等待子进程结束
    if (pclose(fp) == -1)
    {
        perror("pclose failed");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}