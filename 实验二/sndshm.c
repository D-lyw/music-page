#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>

#define BUFSIZE 1024

void main(int argc, char const *argv[])
{
    //定义ga共享内存内部标识shmid
    int shmid;
    //定义附接内存的虚拟地址
    char *viraddr;
    //定义存放信息的字符型数组
    char buffer[BUFSIZE];

    // 创建共享内存
    shmid = shmget(1234, BUFSIZE, 0666|IPC_CREAT);
    // 附接到进程的虚拟地址空间
    viraddr = (char *)shmat(shmid, 0, 0);
    
    puts("Enter some text: ");
    // 循环输入信息
    while(1){
        fgets(buffer, BUFSIZE, stdin);
        strcat(viraddr, buffer);    // 采用追加方式写入到共享内存
        if(strncmp(buffer, "end", 3) == 0)     // 输入为"end"时结束
            break ;
    }

    shmdt(viraddr); //断开附接
    exit(0);
}
