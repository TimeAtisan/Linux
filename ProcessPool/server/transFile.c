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


// 2.0
typedef struct train_s
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
  send(netfd,&train,sizeof(train.length)+train.length,0);

  int fd = open(filename,O_RDWR);
  // 针对小文件
  ssize_t sret = read(fd,train.data,sizeof(train.data));
  train.length = sret;
  send(netfd,&train,sizeof(train.length)+train.length,0);

  return 0;
}