#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void int_func(int sig);
int k;

void int_func(int sig){ // 定义中断后要执行的函数
    k = 0;              // 修改循环变量的值
}

int main(int argc, char const *argv[])
{
    int p; 
    signal(SIGUSR1, int_func);  // 预置信息对应的函数
    k = 1;

    while((p = fork()) == -1);
    if(p == 0){
        while(k == 1);          // 循环等待父进程发软中断信息 
        putchar('B');           // 接收信号后输出'B'
        exit(0);
    }else{
        putchar('A');           // 父进程先输出'A'
        sleep(1);
        kill(p, SIGUSR1);       // 父进程向子进程发软中断信号
        exit(0);
    }
    return 0;
}
