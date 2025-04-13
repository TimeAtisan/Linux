#include <Myhead.h>

// 枚举相当于定义了很多次宏定义
// 记录进程的状态
enum
{
    FREE,
    BUSY
};

#define FREE 0
#define BUSY 1

// 主进程使用保存每个功能进程的信息
typedef struct workerdata_s
{
    pid_t pid;
    int status;
    int pipesockfd; // 父进程和子进程通信的socketpair的fd。
} workerdata_t;

// 第三步 设计函数
// step1:设计函数名字
// 返回值一定要有，可以是int |----> 用于报错
int makeWorker(int workerNum, workerdata_t* workerArr);
int workLoop(int sockfd);

// 线程初始化
int tcpInit(const char* ip, const char* port, int* psockfd);

// epoll相关操作
int epollAdd(int epfd, int fd);
int epollDel(int epfd, int fd);

// 发送netfd给子进程
int sendfd(int sockfd, int fdtosend, int exitFlag);
int recvfd(int sockfd, int* pfdtorecv, int* pexitFlag);

// 业务代码
int transFile(int netfd);