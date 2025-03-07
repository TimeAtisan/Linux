#ifndef MY__HEAD__H
#define MY__HEAD__H

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ARGS_CHECK(argc,num){if(num != argc){fprintf(stdout,"%s","args is error!"); return -1;}}

#define ERROR_CHECK(ret,num,msg){if(ret == num){perror(msg); return -1;}}

#endif