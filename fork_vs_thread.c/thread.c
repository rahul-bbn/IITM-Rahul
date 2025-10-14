#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *thread_func(void *arg){
    int *num=(int *)arg;
    printf("Hello from thread! thread id=%lu  Argument=%d\n",pthread_self(),*num);
    sleep(1);
    printf("thread function pthread finished.\n");
    return NULL;
}

int main(){
    pthread_t thread;
    int value=10;

    // Create a new thread
    pthread_create(&thread,NULL,thread_func,&value);
    printf("hello from main thread! thread id=%lu\n",pthread_self());

    // Wait for the thread to finish
    printf("main thread continues while thread function runs......\n");
    pthread_join(thread,NULL);

    printf("Main thread done.\n");
    return 0;
}