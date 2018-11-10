#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int p1, p2;
    while((p1 = fork()) == -1){} // 创建子进程p1
    if(p1 == 0){
            printf("我是子进程P1\n");
    }else{
        while((p2 = fork()) == -1){} // 创建子进程p2
        if(p2 == 0){
            printf("我是子进程p2,我被成功创建, do something..........\n");
        }else{
            printf("我是父进程,创建子进程失败了...\n");
        }
    }
    return 0;
}
