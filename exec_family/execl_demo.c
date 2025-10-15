#include<stdio.h>
#include<unistd.h>

int main(){
    printf("Before execl()\n");

    execl("/bin/ls","ls","-l",NULL);

    perror("execl failed");

    return 0;
}