#include "taskQueue.h"
#include <Myhead.h>

int taskQueueInit(taskQueue_t *pqueue)
{
  bzero(pqueue,sizeof(taskQueue_t));
  return 0;
}