#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    fork();
    fork();
    fork();
    putchar('A');

    return 0;
}
