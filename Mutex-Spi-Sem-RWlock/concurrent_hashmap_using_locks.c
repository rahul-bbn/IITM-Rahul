#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM_BUCKETS 16

typedef struct Entry {
    char *key;
    int value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry *head;
    pthread_mutex_t lock;
} Bucket;

typedef struct {
    Bucket buckets[NUM_BUCKETS];
} HashMap;

unsigned int hash(const char *key) {
    unsigned int h = 0;
    while (*key) h = h * 31 + (unsigned char)(*key++);
    return h % NUM_BUCKETS;
}

void hashmap_init(HashMap *map) {
    for (int i = 0; i < NUM_BUCKETS; i++) {
        map->buckets[i].head = NULL;
        pthread_mutex_init(&map->buckets[i].lock, NULL);
    }
}

void hashmap_insert(HashMap *map, const char *key, int value) {
    unsigned int idx = hash(key);
    pthread_mutex_lock(&map->buckets[idx].lock);

    Entry *e = map->buckets[idx].head;
    while (e) {
        if (strcmp(e->key, key) == 0) {
            e->value = value;
            pthread_mutex_unlock(&map->buckets[idx].lock);
            return;
        }
        e = e->next;
    }

    Entry *new_e = malloc(sizeof(Entry));
    new_e->key = strdup(key);
    new_e->value = value;
    new_e->next = map->buckets[idx].head;
    map->buckets[idx].head = new_e;

    pthread_mutex_unlock(&map->buckets[idx].lock);
}

int hashmap_lookup(HashMap *map, const char *key, int *out) {
    unsigned int idx = hash(key);
    pthread_mutex_lock(&map->buckets[idx].lock);

    Entry *e = map->buckets[idx].head;
    while (e) {
        if (strcmp(e->key, key) == 0) {
            *out = e->value;
            pthread_mutex_unlock(&map->buckets[idx].lock);
            return 1;
        }
        e = e->next;
    }
    pthread_mutex_unlock(&map->buckets[idx].lock);
    return 0;
}

void *worker(void *arg) {
    HashMap *map = (HashMap *)arg;
    for (int i = 0; i < 100; i++) {
        char key[32];
        sprintf(key, "key%d", i);
        hashmap_insert(map, key, i);
    }
    return NULL;
}

int main() {
    HashMap map;
    hashmap_init(&map);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, worker, &map);
    pthread_create(&t2, NULL, worker, &map);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    int val;
    if (hashmap_lookup(&map, "key42", &val))
        printf("key42 -> %d\n", val);
    else
        printf("key42 not found\n");

    return 0;
}