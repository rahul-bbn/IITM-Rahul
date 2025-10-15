#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>

int main(){
    pid_t pid=fork();

    if(pid<0){
        perror("fork failed");
        exit(1);
    }else if (pid==0)
    {
        printf("Child process: PID = %d and Parent PID = %d\n", getpid(), getppid());

        char *args[]={"date",NULL};
        execvp("date",args);
    }else{
        printf("Parent process: waiting for child to finish...\n");
        wait(NULL);         //NULL means the parent won't collect child's exit status
        printf("Finish child process...\n");
    }
    return 0;
}