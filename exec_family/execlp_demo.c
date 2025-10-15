#include<stdio.h>
#include<unistd.h>

int main(){
    printf("Before execlp\n");

    execlp("ls","ls","-l",NULL);

    perror("execlp failed");

    return 0;
}