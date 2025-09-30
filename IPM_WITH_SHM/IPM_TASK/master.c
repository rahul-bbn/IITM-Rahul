#define _POSIX_C_SOURCE 2008009L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdatomic.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define FTOK_PATH "shmkeyfile"
#define FTOK_PROJID 42
#define MAX_WORKERS 5

typedef struct {
    _Atomic int counters[MAX_WORKERS];  // each worker gets its own counter
} ShmPayload;

static void die(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

int main() {
    FILE *f = fopen(FTOK_PATH, "ab");
    if (!f)
    {
        die("fopen");
    }
    fclose(f);

    key_t key = ftok(FTOK_PATH, FTOK_PROJID);
    if (key == (key_t)-1)
    {
        die("ftok");
    }

    int shmid = shmget(key, sizeof(ShmPayload), IPC_CREAT | 0666);
    if (shmid < 0)
    {
        die("shmget");
    }

    ShmPayload *payload = shmat(shmid, NULL, 0);
    if (payload == (void*)-1)
    {
        die("shmat");
    }

    // Initialize counters
    for (int i = 0; i < MAX_WORKERS; i++) {
        atomic_store(&payload->counters[i], 0);
    }

    printf("[master] Shared memory created (shmid=%d)\n", shmid);

    // Fork workers
    for (int i = 0; i < MAX_WORKERS; i++) {
        pid_t pid = fork();
        if (pid < 0) die("fork");
        if (pid == 0) {
            // Child → worker process
            for (int j = 0; j < 1000; j++) {
                atomic_fetch_add(&payload->counters[i], 1);
            }
            printf("[worker %d] Done. Counter=%d\n", i,
                   atomic_load(&payload->counters[i]));
            _exit(0);
        }
    }

    // Master waits for workers
    for (int i = 0; i < MAX_WORKERS; i++) {
        wait(NULL);
    }

    // Aggregate results
    int total = 0;
    for (int i = 0; i < MAX_WORKERS; i++) {
        int value = atomic_load(&payload->counters[i]);
        printf("[master] Worker %d final counter = %d\n", i, value);
        total += value;
    }
    printf("[master] Aggregated total = %d\n", total);

    // Cleanup
    if (shmdt(payload) == -1)
    {
        die("shmdt");
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        die("shmctl");
    }
    printf("[master] Shared memory removed.\n");

    return 0;
}