#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define NUMBER_OF_PRODUCERS 4
#define NUMBER_OF_CONSUMERS 3
#define PRODUCES_PER_PRODUCER 20
#define BUFFER_SIZE 20

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
int count = 0;
int total_to_be_produced = NUMBER_OF_PRODUCERS*PRODUCES_PER_PRODUCER;
int consumed_total = 0;

void* producer(void *arg){
     int id = *(int *)arg;

     for(int i=0; i<PRODUCES_PER_PRODUCER; i++)
     {
          pthread_mutex_lock(&mutex);
          
          while(count==BUFFER_SIZE)
          {
               printf("Producer %d is waiting....\n", id);
               pthread_cond_wait(&not_full, &mutex);
          }

          int item = i*100 + i;
          buffer[in] = item;
          in = (in+1)%BUFFER_SIZE;
          count++;
          printf("Producer %d Producing.. item produced %d, count %d\n", id, item, count);
          pthread_cond_signal(&not_empty);
          pthread_mutex_unlock(&mutex);
          usleep((rand()%300)*100);
     }
     printf("Producer %d finished producing.\n", id);
     return NULL;
}

void *consumer(void *arg){
     int id = *(int *)arg;

     while(1)
     {
          pthread_mutex_lock(&mutex);

          if(consumed_total>=total_to_be_produced)
          {
               pthread_mutex_unlock(&mutex);
               printf("all task alrady done. exiting....\n");
               break;
          }

          while(count==0)
          {
               printf("Consumer %d, is waiting....\n", id);
               pthread_cond_wait(&not_empty, &mutex);
          }

          int item = buffer[out];
          out = (out + 1)%BUFFER_SIZE;
          count--;
          consumed_total++;
          printf("Consumer %d, is consuming.. item consumed %d, count %d\n", id, item, count);

          pthread_cond_signal(&not_full);
          pthread_mutex_unlock(&mutex);
          usleep((rand()%300)*100);
     }
     printf("Consumer %d finished consuming.\n", id);
     return NULL;
}

int main(){
     pthread_t producers[NUMBER_OF_PRODUCERS], consumers[NUMBER_OF_CONSUMERS];
     int producer_id[NUMBER_OF_PRODUCERS], consumer_id[NUMBER_OF_CONSUMERS];

     for(int i=0; i<NUMBER_OF_PRODUCERS; i++)
     {
          producer_id[i] = i + 1;
          if(pthread_create(&producers[i], NULL, producer, &producer_id[i]) != 0)
          {
               perror("producer pthread create");
               exit(EXIT_FAILURE);
          }
     }

     for(int i=0; i<NUMBER_OF_CONSUMERS; i++)
     {
          consumer_id[i] = i + 1;
          if(pthread_create(&consumers[i], NULL, consumer, &consumer_id[i]) != 0)
          {
               perror("consumer pthread create");
               exit(EXIT_FAILURE);
          }
     }

     for(int i=0; i<NUMBER_OF_PRODUCERS; i++)
     {
          pthread_join(producers[i], NULL);
     }

     pthread_mutex_lock(&mutex);
     pthread_cond_broadcast(&not_empty);
     pthread_mutex_unlock(&mutex);

     for(int i=0; i<NUMBER_OF_CONSUMERS; i++)
     {
          pthread_join(consumers[i], NULL);
     }

     printf("All producer and consumer finished. exiting now....\n");
     return 0;
}