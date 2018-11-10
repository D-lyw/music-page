/*
 * @Author: D-lyw 
 * @Date: 2018-11-10 21:01:36 
 * @Last Modified by: D-lyw
 * @Last Modified time: 2018-11-10 21:23:57
 */
   
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

#define MAXSHM 5                // 定义n缓冲区数组的下标变量个数

// 定义3个n信号量的内部标识
int fullid;
int emptyid;
int mutexid;

int main(int argc, char const *argv[])
{
    // 定义信号量数据结构
    struct sembuf P, V;
    union semun arg;

    // 定义2个共享内存的ID
    int *arrayid;
    int *get;

    // 创建共享内存虚拟地址
    arrayid = shmget(IPC_PRIVATE, sizeof(int )*MAXSHM, IPC_CREAT|0666);
    getid = shmget(IPC_PRIVATE, sizeof(int ), IPC_CREAT|0666);

    // 初始化共享内存
    array = (int *) shmat(arrayid, 0, 0);
    get = (int *) shmat(getid, 0, 0);
    *get = 0;

    // 创建信号量
    fullid = semget(IPC_PRIVATE, 1, IPC_CREAT|0666);
    emptyid = semget(IPC_PRIVATE, 1, IPC_CREAT|0666);
    mutexid = semget (IPC_PRIVATE, 1, IPC_CREAT|0666);

    // 初始化信号量
    arg.val = 0;                        // 初始时缓冲区无数据
    if(semctl(fullid, 0, SETVAL, arg) == -1) 
        perror("Semctl setval error");
    arg.val = MAXSHM;                   // i初始时缓冲区有5个空闲的数组元素
    if(semctl(emptyid, 0, SETVAL, arg) == -1)
        perror("semctl setval error");
    arg.val = 1;                        // 初始时i互斥信号为1, 允许一个进程进入
    if(semctl(mutexid, 0, SETVAL, arg) == -1)
        perror("setctl setval error");
    
    // 初始化P, V操作
    P.sem_num = 0;
    P.sem_op = -1;
    P.sem_flg = SEM_UNDO;
    V.sem_num = 0;
    V.sem_op = 1;
    V.sem_flg = SEM_UNDO;

    // 生产者进程
    if(fork() == 0){
        int i = 0; 
        int set = 0;
        while(i < 10){
            semop(emptyid, &P, 1);          // 对emptyid执行P操作
            semop(mutexid, &P, 1);          // 对muteid执行P操作
            array[set%MAXSHM] = i + 1;
            printf("Producer put number %d to NO. %d \n", arrayp[set%MAXSHM], set%MAXSHM);
            set++;                          // 写计数加1
            semop(mutexid, &V, 1);          // 
        }
    }

    
    return 0;
}
