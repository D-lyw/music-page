#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
    // 定义共享内存内部标识符
    int child, shmid;
    // 定义共享内存附接时的虚拟地址
    char *viraddr;
    // 定义存放信息的字符型数组
    char buffer[BUFSIZE];

    shmid = shmget(IPC_PRIVATE, BUFSIZE, 0666|IPC_CREAT);
    viraddr = (char *)shmat(shmid,0,0);

    while((child = fork()) == -1);
    if(child == 0){
        while(1){
            puts("Enter some text:");
            fgets(buffer, BUFSIZE, stdin);
            strcat(viraddr, buffer);
            if(strncmp(buffer, "end", 3) == 0){
                break;
            }
        }
        exit(0);
    }else{
        wait(0);                                // 等待子进程结束
        printf("Your message is:\n%s", viraddr);    // 输出共享内存中的信息

        shmdt(viraddr);                             // 断开共享内存
        shmctl(shmid, IPC_RMID, 0);                 // 撤销共享内存
        exit(0);
    }
    return 0;
}
