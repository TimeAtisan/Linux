#include <Myhead.h>

// 1.0 版本
/* int recvFile(int sockfd)
{
  char filename[4096] = {0};
  recv(sockfd,filename,sizeof(filename),0);
  int fd = open(filename,O_CREAT | O_RDWR,0666);
  char buf[1000] = {0};
  ssize_t sret = recv(sockfd,buf,sizeof(buf),0);
  write(fd,buf,sret);
  return 0;
} */


// 2.0
/* typedef struct train_s
{
  int length; // 火车头 |---> 如果是8字节对齐则改long
  char data[1000]; // 火车车厢，// 可能存在对齐问题
  // 1000 不是火车车厢的实际大小，是上限。
}train_t;

int recvFile(int sockfd)
{
  char filename[4096] = {0};

  train_t train;
  // 先收火车头，以知道车厢实际长度
  recv(sockfd,&train.length,sizeof(train.length),0);
  recv(sockfd,train.data,train.length,0);
  // 有长度参数保证
  memcpy(filename,train.data,train.length);

  int fd = open(filename,O_CREAT | O_RDWR,0666);
  recv(sockfd,&train.length,sizeof(train.length),0);
  recv(sockfd,train.data,train.length,0);
  write(fd,train.data,train.length);
  close(fd);
  return 0;
} */



// 3.0
typedef struct train_s
{
  int length; // 火车头 |---> 如果是8字节对齐则改long
  char data[1000]; // 火车车厢，// 可能存在对齐问题
  // 1000 不是火车车厢的实际大小，是上限。
}train_t;

int recvFile(int sockfd)
{
  char filename[4096] = {0};

  train_t train;
  // 先收火车头，以知道车厢实际长度
  recv(sockfd,&train.length,sizeof(train.length),MSG_WAITALL);
  recv(sockfd,train.data,train.length,MSG_WAITALL);
  // 有长度参数保证
  memcpy(filename,train.data,train.length);

  int fd = open(filename,O_CREAT | O_RDWR |O_TRUNC,0666);
  while (1)
  {
    recv(sockfd,&train.length,sizeof(train.length),MSG_WAITALL);
    if (train.length != 1000)
    {
      printf("train.length = %d\n",train.length);
    }
    

    if (train.length == 0)
    {
      break;
    }
    
    recv(sockfd,train.data,train.length,MSG_WAITALL);
    write(fd,train.data,train.length);
  }
  
  close(fd);
  return 0;
}


int main(int argc,char *argv[])
{
  ARGS_CHECK(argc,3);

  int sockfd = socket(AF_INET,SOCK_STREAM,0);

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
  serverAddr.sin_port = htons(atoi(argv[2]));
  socklen_t len = sizeof(serverAddr);

  int ret = connect(sockfd,(struct sockaddr *)&serverAddr,len);
  ERROR_CHECK(ret,-1,"connect");
  
  recvFile(sockfd);
  close(sockfd);
  return 0;
}
