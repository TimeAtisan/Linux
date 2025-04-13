#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define ARGS_CHECK(argc, num)                 \
    {                                         \
        if (argc != num)                      \
        {                                     \
            fprintf(stdout, "agrs error!\n"); \
            return -1;                        \
        }                                     \
    }

#define ERROR_CHECK(ret, num, msg) \
    {                              \
        if (num == ret)            \
        {                          \
            perror(msg);           \
            return -1;             \
        }                          \
    }
/*
int main(int argc,char *argv[])
{
  ARGS_CHECK(argc,2);
  int fd = open(argv[1],O_RDWR);
  ERROR_CHECK(fd,-1,"open error!\n");
  char buf[7] = {0};
  ssize_t sret = read(fd,buf,sizeof(buf));
  ERROR_CHECK(sret,-1,"read error!\n");
  fprintf(stdout,"buf = %s ret = %ld\n",buf,sret);
  memset(buf,0,sizeof(buf));
  sret = read(fd,buf,sizeof(buf));
  ERROR_CHECK(sret,-1,"read error!\n");
  fprintf(stdout,"buf = %s ret = %ld\n",buf,sret);
  close(fd);
  return 0;
}
*/
/*
//先写入数据到文件当中
int main(int argc, char *argv[])
{
    // ./write_binary file1
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    int data = 100000;
    ssize_t sret = write(fd,&data,sizeof(int));
    ERROR_CHECK(sret,-1,"write");
    printf("sret = %ld\n", sret);
    close(fd);
    return 0;
}
    */
// 从二进制文件当中进行读取
/*
int main(int argc, char *argv[])
{
    // ./read_binary file1
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    int data;
    ssize_t sret = read(fd,&data,sizeof(int));
    ERROR_CHECK(sret,-1,"read");
    printf("sret = %ld\n", sret);
    ++data;
    printf("data = %d\n", data);
    close(fd);
    return 0;
}
*/
#define SIZE 1024
int cp(char* src, char* dest)
{
    // ./cp src dest
    int rfd = open(src, O_RDONLY);
    ERROR_CHECK(rfd, -1, "open error!");
    int wfd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0774);
    ERROR_CHECK(wfd, -1, "open error!");

    char buf[SIZE] = {0};
    ssize_t ret = 0;
    while ((ret = read(rfd, buf, sizeof(buf))) > 0)
    {
        if (ret < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            perror("read error!");
            exit(-1);
        }

        ssize_t total_written = 0;
        while (total_written < ret)
        {
            ssize_t written = write(wfd, buf, ret - total_written);
            if (written < 0)
            {
                if (errno == EINTR)
                {
                    continue;
                }
                perror("write error!");
                exit(-1);
            }
            total_written += written;
        }
    }
    close(wfd);
    close(rfd);
    return 0;
}

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    cp(argv[1], argv[2]);

    return 0;
}