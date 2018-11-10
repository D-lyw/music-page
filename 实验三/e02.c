/*
 * @Author: D-lyw 
 * @Date: 2018-11-10 20:50:01 
 * @Last Modified by: D-lyw
 * @Last Modified time: 2018-11-10 20:54:33
 * @descripe 利用信号量实现进程同步 
 */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>

union semun{
    int val; 
    struct semid_ds *buf;
    unsigned short *array;
};

// 定义信号量内部标识
int emptyid;
int fullid;

int main(){
    int chld;

    // 定义信号量数据结构
    struct sembuf P, V;
    union semun arg;

    // 定义共享内存
    int shmid;
    char *viraddr;
    char buffer[BUFFER];

    // 创建信号量并初始化
    emptyid = semget(IPC_PRIVATE, 1, IPC_CREAT|0666);
    fullid = semget(IPC_PRIVATE, 1, IPC_CREAT);
    arg.val = 1;
    if(semctl(emptyid, 0, SETVAL, arg) == -1)
        perror("semctl setval error");
    arg.val = 0;
    if(semctl(fullid, 0, SETVAL, arg) == -1)
        perror("semctl setval error");
    
    // 定义P, V 操作
    P.sem_num = 0;
    P.sem_op = -1;
    P.sem_flg = SEM_UNDO;
    V.sem_num = 0;
    V.sem_op = 1;
    V.sem_flg = SEM_UNDO;

    // agni创建并附接共享内存
    shmid =  shmget(IPC_PRIVATE, BUFSIZ, 0666|IPC_CREAT);
    viraddr = (char *)shmat(shmid, 0, 0);

    while ((chld = fork()) == -1);
    if(chld > 0){
        while(1){
            semop(fullid, &P, 1);                       // 对fullidx执行P 操作
            printf("Your message is: \n %s", viraddr);
            semop(emptyid, &V, 1);                      // 对emptyid执行V操作
            if(strncmp(viraddr, "end", 3) == 0)
                break;
        }
        wait(0);
        shmdt(viraddr);
        shmctl(shmid, IPC_RMID, 0);
        semctl(emptyid, IPC_RMID, 0);
        semctl(fullid, IPC_RMID, 0);
        printf("Parent exit!\n");
        exit(0);
    }else{
        while(1){
            semop(emptyid, &P, 1);                      // 对temptyid执行Poo操作
            puts("Enter your text:");
            fgets(buffer, BUFSIZ, stdin);
            strcpy(viraddr, buffer);
            semop(fullid, &V, 1);                       // 对fullidin执行V操作
            if(strncmp(viraddr, "end", 3) == 0){
                sleep(1);                               // 睡眠1秒,等待父进程将"end"取走
                break;
            }
        }
        printf("child exit!\n");
        exit(0);
    }
}
