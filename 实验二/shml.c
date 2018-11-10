#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define MAXMSG 11

int main(int argc, char const *argv[])
{
    int i, chld1, chld2;
    int arrayid;    // 定义共享内存内部标识符
    int *array;     // 定义共享内存附接时的虚拟地址

    // 申请共享内存为具有MAXMSG个元素的整形数组
    arrayid = shmget(IPC_PRIVATE, sizeof(int *)*MAXMSG, IPC_CREAT|0666);
    array = (int *)shmat(arrayid, 0, 0);    // 附接共享内存

    while((chld1 = fork()) == -1);      // 创建子进程
    if(chld1 == 0){                     // 子进程返回
        sleep(1);                       // 睡眠1秒等待子进程写完
        for(i = 0; i <= 10; i++)        
            printf("Your message is: %d\n", array[i]);
        exit(0);
    }
    else{
        while((chld2 = fork()) == -1) ; // 创建子进程
        if(chld2 == 0){                 // 子进程返回
            for(i = 0; i <= 10; i++){
                array[i] = i;           // 写消息到共享内存
                printf("My message is:%d\n", array[i]);
            }
            exit(0);
        }else{                          // 父进程返回
            wait(0);                    // 等待子进程终止
            wait(0);
            shmdt(array);               // 断开附接
            shmctl(arrayid, IPC_RMID, 0);   // 撤销共享内存
            exit(0);
        }
    }
    
    return 0;
}
