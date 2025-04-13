#ifndef __WORKER__H__
#define __WORKER__H__

// c语言中不能重复定义函数不行，全局变量不想
// 可以重复定义结构体，但是结构体要一模一样

#include <Myhead.h>

// 管理子线程
typedef struct tidArr_s
{
    pthread_t* arr;
    int workerNum;
} tidArr_t;

/*
  思维：如何初始化
  参数：
  1. 需要知道多少个子线程
  2. 为子线程分配内存
*/
int tidArrInit(tidArr_t* ptidArr, int workerNum);

void* threadFunc(void* args);

#endif