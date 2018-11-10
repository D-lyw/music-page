#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>

#define BUFSIZE 1024

void main(int argc, char const *argv[])
{
    
    int shmid;
    char *viraddr;

    shmid = shmget(1234, BUFSIZE, 0666|IPC_CREAT); // 获取共享内存
    viraddr = (char *)shmat(shmid, 0, 0); // 附接到进程的虚拟地址空间

    printf("Your message is : \n%s", viraddr);  // 输出信息内容
    shmdt(viraddr);     // 断开附接
    shmctl(shmid, IPC_RMID, 0);     // 撤销共享内存
    
    exit(0);
}
