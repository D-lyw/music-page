#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int pid;
    char *message;
    int n;
    pid = fork();
    if(pid < 0){
        perror("process create failed!\n");
    }else if(pid == 0){
        message = "i am a Child process!\n";
        n = 3;
    }else {
        message = "i am Parent process\n";
        n = 6;
    }
    for(; n > 0; n--){
        printf(message);
        sleep(1);
    }
    return 0;
}
