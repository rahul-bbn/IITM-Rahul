#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<errno.h>
#include<pthread.h>

int shared_var=100;

void *thread_func(void *arg){
    shared_var+=50;
    printf("Inside thread: shared variable=%d\n",shared_var);
    return NULL;
}

int main(){
    printf("Before fork : shared variable=%d\n",shared_var);

    int pid=fork();

    if (pid<0)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid==0)
    {
       shared_var+=10;
       printf("(child process): shared variable=%d\n",shared_var);
       exit(EXIT_SUCCESS);
    }
    else{
        if (wait(NULL)<0)
        {
            perror("wait failed");
            exit(EXIT_FAILURE);
        };
        printf("parent (after fork): shared variable =%d\n",shared_var);

        pthread_t thread;
        int ret;

        ret=pthread_create(&thread,NULL,thread_func,NULL);
        if (ret !=0)
        {
            fprintf(stderr,"Error creating thread: %d\n",ret);
            exit(EXIT_FAILURE);
        }

        ret=pthread_join(thread,NULL);
        if (ret!=0)
        {
            fprintf(stderr,"Error joining thread: %d\n",ret);
            exit(EXIT_FAILURE);
        }
        printf("After thread: shared variable = %d\n",shared_var);
    }
    return 0;
    
}