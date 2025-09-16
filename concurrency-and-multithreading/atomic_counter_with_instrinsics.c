#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

#define THREADS 8
#define ITERATIONS 1000

static long atomic_counter = 0;

typedef struct {
    int id;
    long iterations;
}worker_arg_t;

static void *worker_atomic(void *arg)
{
    worker_arg_t *worker = (worker_arg_t *)arg;

    for (long iterator = 0; iterator < worker->iterations; ++iterator)
    {
        __atomic_fetch_add(&atomic_counter, 1, __ATOMIC_SEQ_CST);
    }

    return NULL;
}

int main(void)
{
    pthread_t threads[THREADS];
    worker_arg_t args[THREADS];

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int iterator = 0; iterator < THREADS; ++iterator)
    {
        args[iterator].id = iterator;
        args[iterator].iterations = ITERATIONS;

        if(pthread_create(&threads[iterator], NULL, worker_atomic, &args[iterator]) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int iterator = 0; iterator < THREADS; ++iterator)
    {
        pthread_join(threads[iterator], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("atomic_counter = %ld (expected = %ld) \n", atomic_counter, (long)THREADS * ITERATIONS);
    printf("time = %.6fs \n", elapsed);

    return 0;
}