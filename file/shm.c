// 获取共享内存段信息
#include <Myhead.h>
int main()
{
    int shmid = shmget(1000, 4096, 0600 | IPC_CREAT); // key为1000 大小为4096 创建一个0600的共享内存
    ERROR_CHECK(shmid, -1, "shmget");
    char* p = (char*) shmat(shmid, NULL, 0);
    ERROR_CHECK(p, (char*) -1, "shmat");
    struct shmid_ds stat;
    int ret = shmctl(shmid, IPC_STAT, &stat);
    ERROR_CHECK(ret, -1, "shmctl");
    printf("cuid = %d perm = %o size = %ld nattch = %ld\n",
           stat.shm_perm.cuid,
           stat.shm_perm.mode,
           stat.shm_segsz,
           stat.shm_nattch);
    return 0;
}