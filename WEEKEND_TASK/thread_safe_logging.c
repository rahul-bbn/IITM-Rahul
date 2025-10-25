/*Implement a thread-safe logging system in C. Create 3 threads that generate log messages simultaneously. Use mutexes to ensure messages are written to a single log file without corruption.*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
FILE *logFile;

void *worker(void *arg){
    int thread_id = *(int *)arg;

    for (int i = 0; i < 5; i++)
    {
        pthread_mutex_lock(&mutex);
        fprintf(logFile,"Thread %d: Log message %d\n",thread_id,i+1);
        fflush(logFile);
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }    
    return NULL;
}

int main(){
    pthread_t t1,t2,t3;
    int id1=1,id2=2,id3=3;

    logFile=fopen("log.txt", "w");
    if (logFile==NULL)
    {
        perror("Error opening log file");
        exit(1);
    }
    
    pthread_create(&t1,NULL,worker,&t1);
    pthread_create(&t2,NULL,worker,&t2);
    pthread_create(&t3,NULL,worker,&t3);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);

    fclose(logFile);
    pthread_mutex_destroy(&mutex);

    printf("All threads finished. Check 'log.txt' for logs.\n");
    return 0;
}