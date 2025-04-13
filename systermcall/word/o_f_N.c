#include "../Myhead.h"

int main()
{
    const char* filename = "test4.txt";

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork失败");
        return 1;
    }

    char buf[1024];
    int ret = 0;

    if (pid > 0)
    {                                                                 // 父进程
        int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644); // 使用 O_APPEND
        if (fd == -1)
        {
            perror("打开文件失败");
            return 1;
        }

        FILE* f1 = fopen("file1.txt", "r");
        if (f1 == NULL)
        {
            perror("打开 file1.txt 失败");
            close(fd);
            return 1;
        }

        ret = fread(buf, 1, sizeof(buf), f1);
        if (ret == -1)
        {
            perror("读取文件失败");
            fclose(f1);
            close(fd);
            return 1;
        }

        write(fd, buf, ret); // 写入父进程数据
        fclose(f1);
        close(fd); // 关闭文件描述符
    }
    else
    {                                                 // 子进程
        sleep(1);                                     // 确保父进程先写
        int fd = open(filename, O_WRONLY | O_APPEND); // 使用 O_APPEND
        if (fd == -1)
        {
            perror("子进程打开文件失败");
            return 1;
        }

        FILE* f2 = fopen("file2.txt", "r");
        if (f2 == NULL)
        {
            perror("打开 file2.txt 失败");
            close(fd);
            return 1;
        }

        ret = fread(buf, 1, sizeof(buf), f2);
        if (ret == -1)
        {
            perror("读取文件失败");
            fclose(f2);
            close(fd);
            return 1;
        }

        write(fd, buf, ret); // 写入子进程数据
        fclose(f2);
        close(fd); // 关闭文件描述符
    }

    // 打印文件内容
    char buffer[128];
    FILE* file = fopen(filename, "r");
    if (file)
    {
        while (fgets(buffer, sizeof(buffer), file))
        {
            printf("%s", buffer);
        }
        fclose(file);
    }
    else
    {
        perror("打开文件以读取内容失败");
    }

    return 0;
}
