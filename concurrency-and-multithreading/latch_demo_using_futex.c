#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdatomic.h>
#include <linux/futex.h>
#include <sys/syscall.h>

// Futex wrappers
static int futex_wait(atomic_int *address, int expected) {
    return syscall(SYS_futex, (int *)address, FUTEX_WAIT | FUTEX_PRIVATE_FLAG, expected, NULL, NULL, 0);
}
static int futex_wake(atomic_int *address, int number) {
    return syscall(SYS_futex, (int *)address, FUTEX_WAKE | FUTEX_PRIVATE_FLAG, number, NULL, NULL, 0);
}

// Latch implementation 
typedef struct {
    atomic_int state; // 0 = free, 1 = held
} latch_t;

static inline void latch_init(latch_t *l) {
    atomic_init(&l->state, 0);
}

// acquire: fast CAS -> spin -> futex wait
void latch_acquire(latch_t *latch) {
    int expected = 0;
    // Fast path: try lock
    if (atomic_compare_exchange_strong_explicit(&latch->state, &expected, 1, memory_order_acq_rel, memory_order_relaxed)) 
    {
        return;
    }

    // Spin a little (short critical sections)
    for (int spin = 0; spin < 100; ++spin)
    {
        if (atomic_load_explicit(&latch->state, memory_order_relaxed) == 0)
        {
            expected = 0;
            if (atomic_compare_exchange_strong_explicit(&latch->state, &expected, 1, memory_order_acq_rel, memory_order_relaxed)) 
            {
                return;
            }
        }
        __asm__ volatile("pause"); 
    }

    // Slow path: futex wait until lock free 
    while (1) {
        expected = 1;
        int return_value = futex_wait(&latch->state, expected);
        if (return_value == -1) {
            if (errno == EAGAIN)
            {
                continue; // value changed before syscall
            }
            if (errno == EINTR) 
            {
                continue;  // interrupted by signal
            }
            perror("futex_wait");
            exit(EXIT_FAILURE);
        }

        // woken: try again 
        expected = 0;
        if (atomic_compare_exchange_strong_explicit(&latch->state, &expected, 1, memory_order_acq_rel, memory_order_relaxed)) 
        {
            return;
        }
    }
}

void latch_release(latch_t *latch) {
    atomic_store_explicit(&latch->state, 0, memory_order_release);
    // wake one waiter 
    if (futex_wake(&latch->state, 1) < 0) {
        perror("futex_wake");
        exit(EXIT_FAILURE);
    }
}

// Shared counter protected by latch
#define NUMBER_OF_THREADS 4
#define INCREMENTS 100000

latch_t latch;
long long shared_counter = 0;

void *worker(void *arg) {
    int id = *(int *)arg;
    for (int iterator = 0; iterator < INCREMENTS; ++iterator) {
        latch_acquire(&latch);

        // critical section
        shared_counter++;

        latch_release(&latch);

        if (iterator % 25000 == 0) {
            printf("Thread %d: progress %d/%d\n", id, iterator, INCREMENTS);
        }
    }
    printf("Thread %d finished\n", id);
    return NULL;
}

int main(void) {
    pthread_t threads[NUMBER_OF_THREADS];
    int ids[NUMBER_OF_THREADS];

    latch_init(&latch);

    for (int iterator = 0; iterator < NUMBER_OF_THREADS; ++iterator) {
        ids[iterator] = iterator + 1;
        if (pthread_create(&threads[iterator], NULL, worker, &ids[iterator]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int iterator = 0; iterator < NUMBER_OF_THREADS; ++iterator) {
        pthread_join(threads[iterator], NULL);
    }

    printf("All threads done. Expected counter=%d, actual=%lld\n", NUMBER_OF_THREADS * INCREMENTS, shared_counter);

    return 0;
}