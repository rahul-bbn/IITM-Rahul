#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 128
#define KEY_LEN 64
#define VAL_LEN 256

typedef struct{
    char key[KEY_LEN];
    char val[VAL_LEN];
} KVPair;

static KVPair store[MAX_ITEMS];
static int store_count = 0;

void set_value(const char *k, const char *v){
    for (int i = 0; i < store_count; i++){
        if (strcmp(store[i].key, k) == 0) {
            strncpy(store[i].val, v, VAL_LEN - 1);
            store[i].val[VAL_LEN - 1] = '\0';
            return;
        }
    }
    if (store_count < MAX_ITEMS) {
        strncpy(store[store_count].key, k, KEY_LEN - 1);
        strncpy(store[store_count].val, v, VAL_LEN - 1);
        store_count++;
    }
}

const char* get_value(const char *k){
    for (int i = 0; i < store_count; i++){
        if (strcmp(store[i].key, k) == 0)
            return store[i].val;
    }
    return NULL;
}
