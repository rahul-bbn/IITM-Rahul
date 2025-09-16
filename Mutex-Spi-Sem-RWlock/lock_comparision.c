#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

typedef enum { LOCK_MUTEX, LOCK_SPIN } lock_type_t;

/* Global sync primitives and counter */
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_spinlock_t g_spin;
static volatile long g_counter = 0;

typedef struct {
    lock_type_t type;
    int iterations;
    int cs_work;              // critical-section work
    pthread_barrier_t *barrier;
} thread_arg_t;

/* less CPU-bound busy work */
static void busy_work(int loops)
{
    volatile unsigned long x = 0;
    for (int i = 0; i < loops; ++i) {
        x += i ^ (x << 1);
    }
}

/* worker that increments the counter under the chosen lock */
static void* worker(void* v)
{
    thread_arg_t* a = (thread_arg_t*) v;

    /* wait until main thread and all other workers are ready */
    pthread_barrier_wait(a->barrier);

    for (int i = 0; i < a->iterations; ++i) {
        if (a->type == LOCK_MUTEX) {
            pthread_mutex_lock(&g_mutex);
            busy_work(a->cs_work);
            g_counter++;
            pthread_mutex_unlock(&g_mutex);
        } else { /* spin */
            pthread_spin_lock(&g_spin);
            busy_work(a->cs_work);
            g_counter++;
            pthread_spin_unlock(&g_spin);
        }
    }

    return NULL;
}

static double timespec_elapsed_sec(const struct timespec* s, const struct timespec* e)
{
    return (double)(e->tv_sec - s->tv_sec) + (e->tv_nsec - s->tv_nsec) / 1e9;
}

/* run one mode (mutex or spin) */
static void run_mode(lock_type_t type, const char* name,
                     int threads, int iterations, int cs_work)
{
    pthread_t *t = malloc(sizeof(pthread_t) * threads);
    if (!t) { perror("malloc"); exit(EXIT_FAILURE); }

    thread_arg_t arg;
    pthread_barrier_t barrier;

    /* initialize barrier for threads + main */
    if (pthread_barrier_init(&barrier, NULL, (unsigned)threads + 1) != 0) {
        perror("pthread_barrier_init");
        free(t);
        exit(EXIT_FAILURE);
    }

    arg.type = type;
    arg.iterations = iterations;
    arg.cs_work = cs_work;
    arg.barrier = &barrier;

    if (type == LOCK_SPIN) {
        if (pthread_spin_init(&g_spin, PTHREAD_PROCESS_PRIVATE) != 0) {
            perror("pthread_spin_init");
            pthread_barrier_destroy(&barrier);
            free(t);
            exit(EXIT_FAILURE);
        }
    }

    g_counter = 0;

    for (int i = 0; i < threads; ++i) {
        int rc = pthread_create(&t[i], NULL, worker, &arg);
        if (rc != 0) {
            fprintf(stderr, "pthread_create failed: %d\n", rc);
            /* best-effort cleanup */
            for (int j = 0; j < i; ++j) pthread_join(t[j], NULL);
            if (type == LOCK_SPIN) pthread_spin_destroy(&g_spin);
            pthread_barrier_destroy(&barrier);
            free(t);
            exit(EXIT_FAILURE);
        }
    }

    /* wait until all workers are ready, then start timing */
    pthread_barrier_wait(&barrier);
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    /* join workers */
    for (int i = 0; i < threads; ++i) {
        pthread_join(t[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);

    double sec = timespec_elapsed_sec(&t0, &t1);
    long total = g_counter;

    printf("%s : threads=%d iterations=%d cs_work=%d -> total=%ld time=%.6fs ops/sec=%.0f\n",
           name, threads, iterations, cs_work, total, sec, total / sec);

    if (type == LOCK_SPIN) {
        pthread_spin_destroy(&g_spin);
    }
    pthread_barrier_destroy(&barrier);
    free(t);
}

int main(int argc, char** argv)
{
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <threads> <iterations> <cs_work> <mode>\n", argv[0]);
        fprintf(stderr, "mode: mutex | spin | both\n");
        return EXIT_FAILURE;
    }

    int threads = atoi(argv[1]);
    int iterations = atoi(argv[2]);
    int cs_work = atoi(argv[3]);
    char mode[16];
    strncpy(mode, argv[4], sizeof(mode) - 1);
    mode[sizeof(mode) - 1] = '\0';

    if (threads <= 0 || iterations <= 0 || cs_work < 0) {
        fprintf(stderr, "Invalid numeric args\n");
        return EXIT_FAILURE;
    }

    if (strcmp(mode, "mutex") == 0 || strcmp(mode, "both") == 0) {
        run_mode(LOCK_MUTEX, "MUTEX", threads, iterations, cs_work);
    }
    if (strcmp(mode, "spin") == 0 || strcmp(mode, "both") == 0) {
        run_mode(LOCK_SPIN, "SPIN", threads, iterations, cs_work);
    }

    pthread_mutex_destroy(&g_mutex);
    return 0;
}
