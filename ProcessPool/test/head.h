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
} workerdata_t;

// 第三步 设计函数
// step1:设计函数名字
// 返回值一定要有，可以是int |----> 用于报错
int makeWorker(int workerNum, workerdata_t* workerArr);