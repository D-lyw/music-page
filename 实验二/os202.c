#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>

#define BUFSIZE 1024

int k;

void int_func(int sig){ // 定义中断后要执行的函数
    k = 0;              // 修改循环变量的值
}

int main(int argc, char const *argv[])
{
    int p; 
    signal(SIGUSR1, int_func);  // 预置信息对应的函数
    k = 1;

    //定义ga共享内存内部标识shmid
    int shmid;
    //定义附接内存的虚拟地址
    char *viraddr_send, *viraddr_recv;
    //定义存放信息的字符型数组
    char buffer[BUFSIZE];

    // 创建共享内存
    shmid = shmget(1234, BUFSIZE, 0666|IPC_CREAT);
    

    while((p = fork()) == -1);

    if(p == 0){
        while(k == 1);                                      // 循环等待父进程发软中断信息 

        viraddr_recv = (char *)shmat(shmid, 0, 0);          // 附接到进程的虚拟地址空间

        printf("Your message is : \n%s", viraddr_recv);          // 输出信息内容
        shmdt(viraddr_recv);                                // 断开附接
        shmctl(shmid, IPC_RMID, 0);                         // 撤销共享内存

        exit(0);
    }else{
        viraddr_send = (char *)shmat(shmid, 0, 0);          // 附接到进程的虚拟地址空间

        puts("Enter some text: ");
        while(1){
            fgets(buffer, BUFSIZE, stdin);
            strcat(viraddr_send, buffer);                    // 采用追加方式写入到共享内存
            if(strncmp(buffer, "end", 3) == 0)               // 输入为"end"时结束
                break;
        }
        shmdt(viraddr_send);                                 //断开附接

        kill(p, SIGUSR1);                                    // 父进程向子进程发软中断信号
        exit(0);
    }
    return 0;
}
