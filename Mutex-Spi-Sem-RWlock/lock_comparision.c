#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>
#include<string.h>


#include <os/lock.h>
typedef os_unfair_lock portable_spinlock_t;
#define portable_spin_init(lock)    (*(lock) = OS_UNFAIR_LOCK_INIT, 0)
#define portable_spin_lock(lock)    os_unfair_lock_lock(lock)
#define portable_spin_unlock(lock)  os_unfair_lock_unlock(lock)
#define portable_spin_destroy(lock) (0)


typedef enum {LOCK_MUTEX, LOCK_SPIN} lock_type_t;

static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static portable_spinlock_t g_spin;
static volatile long g_counter = 0;

typedef struct {
    lock_type_t type;
    int iterations;
    int cs_work; //critical section work
    pthread_barrier_t* barrier;
}thread_arg_t;

//less CPU bound busy work
static void busy_work(int loops)
{
    volatile unsigned long x = 0;
    for (int iterator = 0; iterator < loops; ++iterator)
    {
        x += iterator ^ (x << 1);
    }
}

//worker that increments the counter under the chosen lock
static void* worker(void* v)
{
    thread_arg_t* a = (thread_arg_t*) v;
    pthread_barrier_wait(a->barrier);

    for(int iterator = 0; iterator < a->iterations; ++iterator)
    {
        if(a->type == LOCK_MUTEX)
        {
            pthread_mutex_lock(&g_mutex);
            busy_work(a->cs_work);
            g_counter++;
            pthread_mutex_unlock(&g_mutex);
        }
        else
        {
            portable_spin_lock(&g_spin);
            busy_work(a->cs_work);
            g_counter++;
            portable_spin_unlock(&g_spin);
        }
    }

    return NULL;
}

static double timespec_elapsed_sec(const struct timespec* s, const struct timespec* e)
{
    return (double)(e->tv_sec - s->tv_sec) + (e->tv_nsec - s->tv_nsec) / 1e9;
}

int main(int argc, char** argv)
{
    if(argc < 5)
    {
        fprintf(stderr, "Usage : %s <threads> <iterations> <cs_work> <mode> \n", argv[0]);
        fprintf(stderr, "mode : mutex | spin | both \n");
        return EXIT_FAILURE;
    }

    int threads = atoi(argv[1]);
    int iterations = atoi(argv[2]);
    int cs_work = atoi(argv[3]);
    char mode[16];

    strncpy(mode, argv[4], sizeof(mode) - 1);
    mode[sizeof(mode) - 1] = 0;

    pthread_t *t = malloc(sizeof(pthread_t) * threads);
    thread_arg_t arg;
    pthread_barrier_t barrier;

    auto run_mode = [&](lock_type_t type, const char* name) {
        g_counter = 0;
        arg.type = type;
        if(type == LOCK_SPIN)
        {
            portable_spin_init(&g_spin, PTHREAD_PROCESS_PRIVATE);
        }

        for(int iterator = 0; iterator < threads; ++iterator)
        {
            pthread_create(&t[iterator], NULL, worker, &arg);
        }

        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        pthread_barrier_wait(&barrier); //start all the workers at the same time

        for (int iterator = 0; iterator < threads; ++iterator)
        {
            pthread_join(t[iterator], NULL);
        }
        clock_gettime(CLOCK_MONOTONIC, &t1);

        double sec = timespec_elapsed_sec(&t0, &t1);
        long total = g_counter;

        printf("%s : threads = %d iterations = %d cswork = %d -> total = %ld time = %.6fs ops/sec = %0.f \n",
                name, threads, iterations, cs_work, total, sec, total/sec);

        if(type == LOCK_SPIN)
        {
            pthread_spin_destroy(&g_spin);
        }
    };

    if(strcmp(mode, "mutex") == 0 || strcmp(mode, "both") == 0) run_mode(LOCK_MUTEX, "MUTEX");
    if(strcmp(mode, "spin") == 0 || strcmp(mode, "both") == 0) run_mode(LOCK_SPIN, "SPIN");    
    
    pthread_barrier_destroy(&barrier);
    free(t);
    pthread_mutex_destroy(&g_mutex);

    return 0;

}