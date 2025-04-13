#include "head.h"

// 1.0和1.0 recivefile能对得上。
/*
  大版本号对应的是客户端能否对应的上
  小版本号决定了每一次代码的变动
  对协议
*/
/* int transFile(int netfd)
{
  char filename[] = "file1";
  send(netfd,filename,5,0);
  int fd = open(filename,O_RDWR);
  char buf[1000] = {0};
  ssize_t sret = read(fd,buf,sizeof(buf));
  send(netfd,buf,sret,0);

  return 0;
} */

// 3.0
/* typedef struct train_s
{
  int length; // 4字节对齐 火车头 |---> 如果是8字节对齐则改long
  char data[1000]; // 火车车厢，// 可能存在对齐问题
  // 1000 不是火车车厢的实际大小，是上限。
  // char 数组不是表示一个字符串，他可以保存任何数据  // char占用1一个字节，方便调节实际大小
}train_t;


int transFile(int netfd)
{
  train_t train;
  char filename[] = "file1";
  // 组装小火车
  train.length = strlen(filename);
  memcpy(train.data,filename,train.length);

  // 发送小火车  头长度+车厢长度
  send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);

  int fd = open(filename,O_RDWR);
  while (1)
  {
    // 针对大文件
    ssize_t sret = read(fd,train.data,sizeof(train.data));
    train.length = sret;
    ssize_t sret1 = send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
    if(sret == 0 || sret1 == -1)  // sret1 == -1提前终止避免僵尸进程
    {
      break;
    }
    sleep(1);
  }
  close(fd);


  return 0;
} */

// 4.0
/* typedef struct train_s
{
  int length; // 4字节对齐 火车头 |---> 如果是8字节对齐则改long
  char data[1000]; // 火车车厢，// 可能存在对齐问题
  // 1000 不是火车车厢的实际大小，是上限。
  // char 数组不是表示一个字符串，他可以保存任何数据  // char占用1一个字节，方便调节实际大小
}train_t;


int transFile(int netfd)
{
  train_t train;
  char filename[] = "file1";
  // 组装小火车
  train.length = strlen(filename);
  memcpy(train.data,filename,train.length);

  // 发送小火车  头长度+车厢长度
  send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);


  // 发送文件总长度
  struct stat statbuf;
  int fd = open(filename,O_RDWR);
  fstat(fd,&statbuf);

  // 发送文件的总长度（操作尽量用二进制，文本容易出问题）
  train.length = sizeof(off_t);
  memcpy(train.data,&statbuf.st_size,train.length);
  send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);

  while (1)
  {
    // 针对大文件
    ssize_t sret = read(fd,train.data,sizeof(train.data));
    train.length = sret;
    ssize_t sret1 = send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
    if(sret == 0 || sret1 == -1)  // sret1 == -1提前终止避免僵尸进程
    {
      break;
    }
    // sleep(1);
  }
  close(fd);

  return 0;
} */

// 4.1
/* 使用mmap而不是栈 */
// 1.没有使用read write
// 2.没有使用栈上的车厢了
// 零拷贝技术
/*
  之前使用小火车是因为，不知道文件长度，不能一次发送所有文件
*/
/* typedef struct train_s
{
  int length; // 4字节对齐 火车头 |---> 如果是8字节对齐则改long
  char data[1000]; // 火车车厢，// 可能存在对齐问题
  // 1000 不是火车车厢的实际大小，是上限。
  // char 数组不是表示一个字符串，他可以保存任何数据  // char占用1一个字节，方便调节实际大小
}train_t;


int transFile(int netfd)
{
  train_t train;
  char filename[] = "file1";
  // 组装小火车
  train.length = strlen(filename);
  memcpy(train.data,filename,train.length);

  // 发送小火车  头长度+车厢长度
  send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);


  // 发送文件总长度
  struct stat statbuf;
  int fd = open(filename,O_RDWR); // 1.open 必须用O_RDWR
  fstat(fd,&statbuf);

  // 发送文件的总长度（操作尽量用二进制，文本容易出问题）
  train.length = sizeof(off_t);
  memcpy(train.data,&statbuf.st_size,train.length);
  send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);

  // 2.使用mmap
  char *p = (char *)mmap(NULL,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

  ERROR_CHECK(p,MAP_FAILED,"mmap");
  off_t cursize = 0; // 已经发送的长度
  while (1)
  {
    if(cursize >= statbuf.st_size)
    {
      break;
    }

    if(statbuf.st_size - cursize >= 1000)
    {
      train.length = 1000;
    }
    else
    {
      train.length = statbuf.st_size - cursize;
    }
    send(netfd,&train.length,sizeof(train.length),MSG_NOSIGNAL);
    // 不要发货车车厢，而是发映射区
    send(netfd,p+cursize,train.length,MSG_NOSIGNAL);
    cursize += train.length;
  }

  train.length = 0;
  send(netfd,&train.length,sizeof(train.length),MSG_NOSIGNAL);
  munmap(p,statbuf.st_size);

  close(fd);

  return 0;
} */

// 5.0使用大货车
typedef struct train_s
{
    int length;      // 4字节对齐 火车头 |---> 如果是8字节对齐则改long
    char data[1000]; // 火车车厢，// 可能存在对齐问题
                     // 1000 不是火车车厢的实际大小，是上限。
                     // char 数组不是表示一个字符串，他可以保存任何数据  // char占用1一个字节，方便调节实际大小
} train_t;

int transFile(int netfd)
{
    train_t train;
    char filename[] = "file1";
    // 组装小火车
    train.length = strlen(filename);
    memcpy(train.data, filename, train.length);

    // 发送小火车  头长度+车厢长度
    send(netfd, &train, sizeof(train.length) + train.length, MSG_NOSIGNAL);

    // 发送文件总长度
    struct stat statbuf;
    int fd = open(filename, O_RDWR); // 1.open 必须用O_RDWR
    fstat(fd, &statbuf);

    // 发送文件的总长度（操作尽量用二进制，文本容易出问题）
    train.length = sizeof(off_t);
    memcpy(train.data, &statbuf.st_size, train.length);
    send(netfd, &train, sizeof(train.length) + train.length, MSG_NOSIGNAL);

    // 2.使用mmap
    /* char *p = (char *)mmap(NULL,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    ERROR_CHECK(p,MAP_FAILED,"mmap");

    send(netfd,p,statbuf.st_size,MSG_NOSIGNAL);

    munmap(p,statbuf.st_size); */

    sendfile(netfd, fd, NULL, statbuf.st_size);

    close(fd);

    return 0;
}