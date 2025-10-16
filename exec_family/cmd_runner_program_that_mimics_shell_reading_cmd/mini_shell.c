#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <sys/wait.h>

#define MAX_INPUT 100
#define MAX_ARGS 10

int main(){
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    int nbytes;

    while (1)
    {
        printf("my_shell> ");
        fflush(stdout);

        nbytes=read(0,input,sizeof(input)-1);

        if (nbytes<=0)
        {
            printf("\n");
            break;
        }

        input[nbytes]='\0';

        input[strcspn(input,"\n")]='\0';

        if (strcmp(input,"exit")==0)
        {
            break;
        }
        // Tokenize input into arguments
         int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL && i < MAX_ARGS - 1)
            args[++i] = strtok(NULL, " ");
        args[i] = NULL;

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            if (execvp(args[0], args) == -1) {
                perror("exec failed");
            }
            exit(EXIT_FAILURE);
        } else {
            wait(NULL);    //NULL means the parent won't collect child's exit status
        }
    }

    printf("Exiting shell...\n");
    return 0;
}