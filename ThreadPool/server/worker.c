#include "worker.h"

int tidArrInit(tidArr_t *ptidArr,int workerNum)
{
  // 申请内存存储每个子线程的tid
  ptidArr->arr = (pthread_t *)calloc(workerNum,sizeof(pthread_t));
  ptidArr->workerNum = workerNum;

  return 0;
}

void* threadFunc(void *args)
{
  while (1)
  {
    sleep(1);
  }
  
}