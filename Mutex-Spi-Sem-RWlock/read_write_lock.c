#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define READERS 5
#define WRITERS 2

pthread_rwlock_t rwlock;  // read-write lock
int shared_data = 0;      // shared resource

// Reader thread
void* reader(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pthread_rwlock_rdlock(&rwlock);   // lock for reading
        printf("Reader %d: read value = %d\n", id, shared_data);
        pthread_rwlock_unlock(&rwlock);   // unlock
        sleep(1);
    }
    return NULL;
}

// Writer thread
void* writer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pthread_rwlock_wrlock(&rwlock);   // lock for writing
        shared_data++;
        printf("Writer %d: wrote value = %d\n", id, shared_data);
        pthread_rwlock_unlock(&rwlock);   // unlock
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t r[READERS], w[WRITERS];
    int r_id[READERS], w_id[WRITERS];

    pthread_rwlock_init(&rwlock, NULL);

    // create readers
    for (int i = 0; i < READERS; i++) {
        r_id[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &r_id[i]);
    }

    // create writers
    for (int i = 0; i < WRITERS; i++) {
        w_id[i] = i + 1;
        pthread_create(&w[i], NULL, writer, &w_id[i]);
    }

    // join (infinite loop, so won't reach)
    for (int i = 0; i < READERS; i++) pthread_join(r[i], NULL);
    for (int i = 0; i < WRITERS; i++) pthread_join(w[i], NULL);

    pthread_rwlock_destroy(&rwlock);
    return 0;
}
