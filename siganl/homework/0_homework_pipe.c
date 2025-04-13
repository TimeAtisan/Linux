#include <Myhead.h>

int main(int argc, const char* argv[])
{
    int fds[2];
    pipe(fds);
    // 管道没有写入，读管道阻塞
    /*
    if(fork())
    {
      close(fds[0]);
      sleep(6);
      printf("sleep over!\n");
      write(fds[1],"hello",5);
    }else
    {
      close(fds[1]);
      char buf[4096];
      ssize_t sret = read(fds[0],buf,4096);
      printf("sret = %ld,buf = %s\n",sret,buf);
    }
    */

    // 写快读慢会出现写阻塞
    /*
    if(fork())
    {
      close(fds[0]);
      char buf[4096];
      int cnt = 0;
      while (1)
      {
        write(fds[1],buf,4096);
        printf("parent write cnt = %d\n",cnt++);
        sleep(1);
      }

    }
    else
    {
      close(fds[1]);
      char buf[4096];
      int cnt = 0;
      while (1)
      {
        read(fds[0],buf,4096);
        printf("child read cnt = %d\n",cnt++);
        sleep(1);
      }
    }
    */

    // 先关闭读端，写端继续写入,写进程异常退出
    /*
    if(fork())
    {
      close(fds[0]);
      sleep(5);
      printf("sleep over!\n");
      write(fds[1],"hello",5);
      printf("can you see me !");
      wait(NULL);
    }
    else
    {
      close(fds[1]);
      close(fds[0]);
      while (1)
      {
        sleep(1);
      }

    }
    */
    // 想关闭写端，读端继续,read返回0
    if (fork())
    {
        close(fds[0]);
        close(fds[1]);
        printf("sleep over!\n");
        write(fds[1], "hello", 5);
        printf("can you see me !");
        wait(NULL);
    }
    else
    {
        close(fds[1]);
        sleep(1);
        puts("sleep over!");
        char buf[4096] = {0};
        size_t sret = read(fds[0], buf, sizeof(buf));
        printf("sret = %ld,buf = %s\n", sret, buf);
    }
    return 0;
}
