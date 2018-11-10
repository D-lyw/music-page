/*
 * @Author: D-lyw 
 * @Date: 2018-11-07 14:37:45 
 * @Last Modified by: D-lyw
 * @Last Modified time: 2018-11-10 20:59:36
 * @Descripe 利用信号量实现进程互斥的实例
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

//定义信号量标识
int mutexid;        

int main(int argc, char const *argv[])
{
    int chld, i, j;

    // 定义数据结构
    struct sembuf P, V;
    union semun arg;

    // 创建只含g一二互斥信号量u元素的信号量集
    mutexid = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
    // 为g信号量赋初值
    arg.val = 1;
    if(semctl(mutexid, 0, SETVAL, arg) == -1)
        perror("semctl setval error\n");
    
    // 定义P, V 操作
    P.sem_num = 0;
    P.sem_op = -1;
    P.sem_flg = SEM_UNDO;
    V.sem_num = 0;
    V.sem_op = 1;
    V.sem_flg = SEM_UNDO;

    while((chld = fork()) == -1);       // 创建子进程
    if(chld > 0){                       // 父进程返回
        i = 1;
        while(i <= 3){
            sleep(1);
            semop(mutexid, &P, 1);      // 进入临界区前执行P(mutex)
            printf("prnt in \n");
            sleep(1);
            printf("prnt out\n");
            semop(mutexid, &V, 1);      // 出临界区执行V(mutex)
            i++;
        }
        wait(0);                        // 等待子进程g终止
        semctl(mutexid, IPC_RMID, 0);   // 撤销信号量
        exit(0);
    }else{                              // 子进程返回
        j = 1;
        while(j <= 3){
            sleep(1);
            semop(mutexid, &P, 1);      // 进入临界区前执行P(mutex)
            printf("chld in\n");
            sleep(1);
            printf("chld out\n");
            semop(mutexid, &V, 1);      // 出临界区执行V(mutex)
            j++;
        }
        exit(0);
    }
    
    return 0;
}
