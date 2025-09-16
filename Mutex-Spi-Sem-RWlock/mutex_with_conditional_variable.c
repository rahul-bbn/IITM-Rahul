#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
#define PRODUCER_PER_PRODUCER 10
#define NUMBER_OF_PRODUCERS 2
#define NUMBER_OF_CONSUMERS 2

/* shared buffer (circular) */
int buffer[BUFFER_SIZE];
int in = 0;      // insert index
int out = 0;     // remove index
int count = 0;   // number of items in the buffer

/* helpers for termination */
int total_to_produce = PRODUCER_PER_PRODUCER * NUMBER_OF_PRODUCERS;
int consumed_total = 0;

/* sync primitives */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full  = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

/* producer */
void *producer(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < PRODUCER_PER_PRODUCER; i++) {
        usleep((rand() % 200) * 1000);     //0-200ms

        pthread_mutex_lock(&mutex);
        while (count == BUFFER_SIZE) {
            printf("Producer %d: buffer full, waiting...\n", id);
            pthread_cond_wait(&not_full, &mutex);
        }

        int item = id * 1000 + i;
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        ++count;

        printf("Producer %d: produced %d (count=%d)\n", id, item, count);

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }

    printf("Producer %d: done producing.\n", id);
    return NULL;
}

/* consumer */
void *consumer(void *arg) {
    int id = *(int *)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == 0) {
            if (consumed_total >= total_to_produce) {
                pthread_mutex_unlock(&mutex);
                printf("Consumer %d: no more items, exiting...\n", id);
                return NULL;
            }
            printf("Consumer %d: buffer empty, waiting...\n", id);
            pthread_cond_wait(&not_empty, &mutex);
        }

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        --count;
        ++consumed_total;

        printf("Consumer %d: consumed %d (consumed_total=%d, count=%d)\n",
               id, item, consumed_total, count);

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);

        usleep((rand() % 300) * 1000);      //0-300ms

        if (consumed_total >= total_to_produce) {
            printf("Consumer %d: consumed final item, exiting...\n", id);
            return NULL;
        }
    }
}

/* main */
int main(void) {
    srand((unsigned)time(NULL));

    pthread_t producers[NUMBER_OF_PRODUCERS], consumers[NUMBER_OF_CONSUMERS];
    int ids_producers[NUMBER_OF_PRODUCERS], ids_consumers[NUMBER_OF_CONSUMERS];

    for (int i = 0; i < NUMBER_OF_PRODUCERS; i++) {
        ids_producers[i] = i + 1;
        if (pthread_create(&producers[i], NULL, producer, &ids_producers[i]) != 0) {
            perror("pthread_create producer");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUMBER_OF_CONSUMERS; i++) {
        ids_consumers[i] = i + 1;
        if (pthread_create(&consumers[i], NULL, consumer, &ids_consumers[i]) != 0) {
            perror("pthread_create consumer");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUMBER_OF_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&not_empty);
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < NUMBER_OF_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    printf("\nAll producers and consumers finished.\n");
    printf("Consumed total = %d (Expected = %d)\n", consumed_total, total_to_produce);

    return 0;
}