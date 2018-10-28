#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

pthread_t ntid;

void printids(const char *s){
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %u tid (0x%x)\n", s, (unsigned int)pid, (unsigned int)tid, (unsigned int)tid);
}

void *thr_fn(void *arg){
    printids(arg);
    return NULL;
}

int main(int argc, char const *argv[])
{
    int err;

    err = pthread.create(&ntid, NULL, thr_fn, "new: thread: ");
    if(err != 0){
        fprintf(stderr, "can't create thread: %s\n", streerror(err));
    }
    printids("main thread: ");
    sleep(1);
    return 0;
}
