#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

static long atomic_counter = 0;
static long mutex_counter = 0;
static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    long iterations;
}bench_arg_t;

static void barrier_wait(volatile int *barrier)
{
    __sync_fetch_and_add(barrier, 1);
    while (*barrier != 0 )
    {
        // spin until the main resets
    }
}

static void *worker_atomic(void *arg)
{
    bench_arg_t *arguement = (bench_arg_t *) arg;
    long iterations = arguement->iterations;

    for (long iterator = 0; iterator < iterations; ++iterator)
    {
        __atomic_fetch_add(&atomic_counter, 1, __ATOMIC_RELAXED);
    }

    return NULL;
}

static void *worker_mutex(void *arg)
{
    bench_arg_t *arguement = (bench_arg_t *) arg;
    long iterations = arguement->iterations;

    for (long iterator = 0; iterator < iterations; ++iterator)
    {
        pthread_mutex_lock(&counter_mutex);
        ++mutex_counter;
        pthread_mutex_unlock(&counter_mutex);
    }

    return NULL;
}

static double convert_to_seconds(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9;
}

int main(int argc, char **argv)
{
    int number_of_threads = (argc > 1) ? atoi(argv[1]) : 32;
    long iterations = (argc > 2) ? atol (argv[2]) : 1000000;

    pthread_t *threads = malloc(sizeof(pthread_t) * number_of_threads);
    bench_arg_t arg = { iterations };

    //atomic counter benchmark
    atomic_counter = 0;
    double start_atomic = convert_to_seconds();

    for(int iterator = 0; iterator < number_of_threads; ++iterator)
    {
        pthread_create(&threads[iterator], NULL, worker_atomic, &arg);
    }

    for(int iterator = 0; iterator < number_of_threads; ++iterator)
    {
        pthread_join(threads[iterator], NULL);
    }

    double end_atomic = convert_to_seconds();

    printf("Atomic : counter = %ld time = %.6fs \n", atomic_counter, end_atomic - start_atomic);

    //mutex counter benchmark
    mutex_counter = 0;
    double start_mutex = convert_to_seconds();

    for(int iterator = 0; iterator < number_of_threads; ++iterator)
    {
        pthread_create(&threads[iterator], NULL, worker_mutex, &arg);
    }

    for(int iterator = 0; iterator < number_of_threads; ++iterator)
    {
        pthread_join(threads[iterator], NULL);
    }

    double end_mutex = convert_to_seconds();

    printf("Mutex : counter = %ld time = %.6fs \n", mutex_counter, end_mutex - start_mutex);\

    free(threads);
    return 0;
}