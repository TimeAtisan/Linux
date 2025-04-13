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
} */

// 4.0
/* typedef struct train_s
{
  int length; // 火车头 |---> 如果是8字节对齐则改long
  char data[1000]; // 火车车厢，// 可能存在对齐问题
  // 1000 不是火车车厢的实际大小，是上限。
}train_t;


int print_progress_bar(int cursize, int filesize) {
  int bar_width = 50; // 进度条的宽度
  float ratio = (float) (cursize * 1.0/ filesize);
  int filled_width = (int)(ratio * bar_width);

  printf("\r[");
  for (int i = 0; i < filled_width; i++) {
        printf("=");
    }
  for (int i = filled_width; i < bar_width; i++) {
      printf(" ");
  }

  printf("] %5.2lf%%",(ratio * 100));
  fflush(stdout); // 确保立即输出
}

int recvFile(int sockfd)
{
  char filename[4096] = {0};

  train_t train;
  // 先收火车头，以知道车厢实际长度
  recv(sockfd,&train.length,sizeof(train.length),MSG_WAITALL);
  recv(sockfd,train.data,train.length,MSG_WAITALL);
  // 有长度参数保证
  memcpy(filename,train.data,train.length);

  off_t filesize;

  off_t lastsize = 0; // 上次printf之后文件的大小
  off_t slice = filesize / 10000; // 每次打印前后相差0.01%才更新一次。最小分片

  recv(sockfd,&train.length,sizeof(train.length),MSG_WAITALL);
  recv(sockfd,train.data,train.length,MSG_WAITALL);
  memcpy(&filesize,train.data,train.length);
  printf("filesize = %ld\n",filesize);

  off_t cursize = 0;
  int fd = open(filename,O_CREAT | O_RDWR |O_TRUNC,0666);

  while (1)
  {
    recv(sockfd,&train.length,sizeof(train.length),MSG_WAITALL);
    /* if (train.length != 1000)
    {
      printf("\n");
      printf("train.length = %d\n",train.length);
    } */

/*
    if (train.length == 0)
    {
      break;
    }

    cursize += train.length;

    recv(sockfd,train.data,train.length,MSG_WAITALL);
    write(fd,train.data,train.length);
    // \r光标回到开头位置，相当于覆盖之前的数据
    /* printf("%5.2lf%%\r",cursize*100.0/filesize); // 一定等于0，因为整数除以整数一定是整数 */
/*
    if (cursize - lastsize > slice)
    {
      print_progress_bar(cursize,filesize);
      lastsize = cursize;
    }

    usleep(1000);

    // 不加\n则无法进行缓冲区刷新,手动刷新缓冲区
    // fflush(stdout);
  }
  printf("\nfinish!\n");
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
} */

// 4.1

/* typedef struct train_s
{
  int length; // 火车头 |---> 如果是8字节对齐则改long
  char data[1000]; // 火车车厢，// 可能存在对齐问题
  // 1000 不是火车车厢的实际大小，是上限。
}train_t;


int print_progress_bar(int cursize, int filesize) {
  int bar_width = 50; // 进度条的宽度
  float ratio = (float) (cursize * 1.0/ filesize);
  int filled_width = (int)(ratio * bar_width);

  printf("\r[");
  for (int i = 0; i < filled_width; i++) {
        printf("=");
    }
  for (int i = filled_width; i < bar_width; i++) {
      printf(" ");
  }

  printf("] %5.2lf%%",(ratio * 100));
  fflush(stdout); // 确保立即输出
}

int recvFile(int sockfd)
{
  char filename[4096] = {0};

  train_t train;
  // 先收火车头，以知道车厢实际长度
  recv(sockfd,&train.length,sizeof(train.length),MSG_WAITALL);
  recv(sockfd,train.data,train.length,MSG_WAITALL);
  // 有长度参数保证
  memcpy(filename,train.data,train.length);

  off_t filesize;

  off_t lastsize = 0; // 上次printf之后文件的大小
  off_t slice = filesize / 10000; // 每次打印前后相差0.01%才更新一次。最小分片

  recv(sockfd,&train.length,sizeof(train.length),MSG_WAITALL);
  recv(sockfd,train.data,train.length,MSG_WAITALL);
  memcpy(&filesize,train.data,train.length);
  printf("filesize = %ld\n",filesize);

  off_t cursize = 0;
  int fd = open(filename,O_CREAT | O_RDWR |O_TRUNC,0666);

  while (1)
  {
    recv(sockfd,&train.length,sizeof(train.length),MSG_WAITALL);
    /* if (train.length != 1000)
    {
      printf("\n");
      printf("train.length = %d\n",train.length);
    } */

/*
    if (train.length == 0)
    {
      break;
    }

    cursize += train.length;

    recv(sockfd,train.data,train.length,MSG_WAITALL);
    write(fd,train.data,train.length);
    // \r光标回到开头位置，相当于覆盖之前的数据
    /* printf("%5.2lf%%\r",cursize*100.0/filesize); // 一定等于0，因为整数除以整数一定是整数 */
/*
    if (cursize - lastsize > slice)
    {
      print_progress_bar(cursize,filesize);
      lastsize = cursize;
    }

    usleep(1000);

    // 不加\n则无法进行缓冲区刷新,手动刷新缓冲区
    // fflush(stdout);
  }
  printf("\nfinish!\n");
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
} */

// 4.2
/* typedef struct train_s
{
  int length; // 火车头 |---> 如果是8字节对齐则改long
  char data[1000]; // 火车车厢，// 可能存在对齐问题
  // 1000 不是火车车厢的实际大小，是上限。
}train_t;

int recvn(int sockfd,void *buf,long total)
{
  char *p = (char *)buf;
  long cursize = 0;
  while (cursize < total)
  {
    ssize_t sret = recv(sockfd,p+cursize,total-cursize,0);
    if (sret == 0)
    {
      return 1;
    }
    cursize += sret;
  }
  return 0;
}


int print_progress_bar(int cursize, int filesize) {
  int bar_width = 50; // 进度条的宽度
  float ratio = (float) (cursize * 1.0/ filesize);
  int filled_width = (int)(ratio * bar_width);

  printf("\r[");
  for (int i = 0; i < filled_width; i++) {
        printf("=");
    }
  for (int i = filled_width; i < bar_width; i++) {
      printf(" ");
  }

  printf("] %5.2lf%%",(ratio * 100));
  fflush(stdout); // 确保立即输出
}

int recvFile(int sockfd)
{
  char filename[4096] = {0};

  train_t train;
  /* // 先收火车头，以知道车厢实际长度
  recv(sockfd,&train.length,sizeof(train.length),MSG_WAITALL);
  recv(sockfd,train.data,train.length,MSG_WAITALL); */
/*
  recvn(sockfd,&train.length,sizeof(train.length));
  recvn(sockfd,train.data,train.length);

  // 有长度参数保证
  memcpy(filename,train.data,train.length);

  off_t filesize;

  off_t lastsize = 0; // 上次printf之后文件的大小
  off_t slice = filesize / 10000; // 每次打印前后相差0.01%才更新一次。最小分片

  recvn(sockfd,&train.length,sizeof(train.length));
  recvn(sockfd,train.data,train.length);
  memcpy(&filesize,train.data,train.length);
  printf("filesize = %ld\n",filesize);

  off_t cursize = 0;
  int fd = open(filename,O_CREAT | O_RDWR |O_TRUNC,0666);

  while (1)
  {

    recvn(sockfd,&train.length,sizeof(train.length));
    /* recv(sockfd,&train.length,sizeof(train.length),MSG_WAITALL); */
/* if (train.length != 1000)
{
  printf("\n");
  printf("train.length = %d\n",train.length);
} */

/*
    if (train.length == 0)
    {
      break;
    }

    cursize += train.length;

    recvn(sockfd,train.data,train.length);
    write(fd,train.data,train.length);
    // \r光标回到开头位置，相当于覆盖之前的数据
    /* printf("%5.2lf%%\r",cursize*100.0/filesize); // 一定等于0，因为整数除以整数一定是整数 */
/*
    if (cursize - lastsize > slice)
    {
      print_progress_bar(cursize,filesize);
      lastsize = cursize;
    }

    usleep(1000);

    // 不加\n则无法进行缓冲区刷新,手动刷新缓冲区
    // fflush(stdout);
  }
  printf("\nfinish!\n");
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
} */

typedef struct train_s
{
    int length;      // 火车头 |---> 如果是8字节对齐则改long
    char data[1000]; // 火车车厢，// 可能存在对齐问题
                     // 1000 不是火车车厢的实际大小，是上限。
} train_t;

int recvn(int sockfd, void* buf, long total)
{
    char* p = (char*) buf;
    long cursize = 0;
    while (cursize < total)
    {
        ssize_t sret = recv(sockfd, p + cursize, total - cursize, 0);
        if (sret == 0)
        {
            return 1;
        }
        cursize += sret;
    }
    return 0;
}

int print_progress_bar(int cursize, int filesize)
{
    int bar_width = 50; // 进度条的宽度
    float ratio = (float) (cursize * 1.0 / filesize);
    int filled_width = (int) (ratio * bar_width);

    printf("\r[");
    for (int i = 0; i < filled_width; i++)
    {
        printf("=");
    }
    for (int i = filled_width; i < bar_width; i++)
    {
        printf(" ");
    }

    printf("] %5.2lf%%", (ratio * 100));
    fflush(stdout); // 确保立即输出
}

int recvFile(int sockfd)
{
    char filename[4096] = {0};

    train_t train;
    /* // 先收火车头，以知道车厢实际长度
    recv(sockfd,&train.length,sizeof(train.length),MSG_WAITALL);
    recv(sockfd,train.data,train.length,MSG_WAITALL); */

    recvn(sockfd, &train.length, sizeof(train.length));
    recvn(sockfd, train.data, train.length);

    // 有长度参数保证
    memcpy(filename, train.data, train.length);

    off_t filesize;
    recvn(sockfd, &train.length, sizeof(train.length));
    recvn(sockfd, train.data, train.length);
    memcpy(&filesize, train.data, train.length);
    printf("filesize = %ld\n", filesize);

    int fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0666);

    // 使用mmap之前为什么要使用ftruncate，
    ftruncate(fd, filesize); // 先修改文件大小，不然mmap会失败

    char* p = (char*) mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    ERROR_CHECK(p, MAP_FAILED, "mmap");

    recvn(sockfd, p, filesize);

    munmap(p, filesize);
    close(fd);
    return 0;
}

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr.sin_port = htons(atoi(argv[2]));
    socklen_t len = sizeof(serverAddr);

    int ret = connect(sockfd, (struct sockaddr*) &serverAddr, len);
    ERROR_CHECK(ret, -1, "connect");

    recvFile(sockfd);
    close(sockfd);
    sleep(20);
    return 0;
}