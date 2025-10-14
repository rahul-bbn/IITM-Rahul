#include<stdio.h>
#include<unistd.h>

int main(){
    printf("program started:%d\n",getpid());

    int pid=fork();

    if (pid<0)
    {
       perror("fork failed:");
    }else if (pid==0)
    {
        printf("Child process: child pid=%d and parent pid=%d",getpid(),getppid());
    }else if (pid>0)
    {
        printf("Parent process: parent pid=%d and child pid=%d",getpid(),pid);
    }
    
    printf("\nprogram exiting....\n");

    return 0;
    
}