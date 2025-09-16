#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define THREADS 8
#define OPS_PER_THREAD 100000

static long counter = 0;
static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

//worker with mutex
static void* worker_with_mutex(void* arg)
{
    (void)arg;

    for (int iterator = 0; iterator < OPS_PER_THREAD; ++iterator)
    {
        pthread_mutex_lock(&counter_mutex);
        counter++;
        pthread_mutex_unlock(&counter_mutex);
    }

    return NULL;
}

//worker without mutex
static void* worker_without_mutex(void* arg)
{
    (void)arg;

    for (int iterator = 0; iterator < OPS_PER_THREAD; ++iterator)
    {
        counter++;
    }

    return NULL;
}

int main(void)
{
    pthread_t thread[THREADS];

    //without mutex
    counter = 0;
    for (int iterator = 0; iterator < THREADS; ++iterator)
    {
        pthread_create(&thread[iterator], NULL, worker_without_mutex, NULL);
    }

    for (int iterator = 0; iterator < THREADS; ++iterator)
    {
        pthread_join(thread[iterator], NULL);
    }

    printf("Without mutex : expected = %d actual = %ld \n", THREADS * OPS_PER_THREAD, counter);

    //with mutex
    counter = 0;
    for (int iterator = 0; iterator < THREADS; ++iterator)
    {
        pthread_create(&thread[iterator], NULL, worker_with_mutex, NULL);
    }

    for (int iterator = 0; iterator < THREADS; ++iterator)
    {
        pthread_join(thread[iterator], NULL);
    }

    printf("With mutex : expected = %d actual = %ld \n", THREADS * OPS_PER_THREAD, counter);

    pthread_mutex_destroy(&counter_mutex);

    return 0;
}