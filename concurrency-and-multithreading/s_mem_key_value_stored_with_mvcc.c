#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_KEYS 128
#define MAX_KEY_LEN 64
#define MAX_VALUE_LEN 256
#define MAX_TXNS 256

typedef unsigned long ts_t;
typedef int txnid_t;

typedef struct Version {
    char value[MAX_VALUE_LEN];
    ts_t commit_ts;            
    struct Version *next;      
} Version;

typedef struct KeyEntry {
    char key[MAX_KEY_LEN];
    Version *versions;         
    pthread_mutex_t mtx;       
    int used;
} KeyEntry;

KeyEntry store[MAX_KEYS];
pthread_mutex_t store_mtx = PTHREAD_MUTEX_INITIALIZER;

static int hash_key(const char *k) {
    unsigned long h = 5381;
    for (const char *p = k; *p; ++p) h = ((h << 5) + h) + (unsigned char)(*p);
    return (int)(h % MAX_KEYS);
}

static int get_or_create_key(const char *k) {
    int idx = hash_key(k);
    int start = idx;
    pthread_mutex_lock(&store_mtx);
    while (1) {
        if (!store[idx].used) {
            strncpy(store[idx].key, k, MAX_KEY_LEN-1);
            store[idx].key[MAX_KEY_LEN-1] = '\0';
            store[idx].versions = NULL;
            pthread_mutex_init(&store[idx].mtx, NULL);
            store[idx].used = 1;
            pthread_mutex_unlock(&store_mtx);
            return idx;
        } else if (strncmp(store[idx].key, k, MAX_KEY_LEN) == 0) {
            pthread_mutex_unlock(&store_mtx);
            return idx;
        }
        idx = (idx + 1) % MAX_KEYS;
        if (idx == start) {
            pthread_mutex_unlock(&store_mtx);
            fprintf(stderr, "Key store full\n");
            exit(1);
        }
    }
}

typedef enum { TXN_ACTIVE, TXN_ABORTED, TXN_COMMITTED } txn_status_t;

typedef struct WriteEntry {
    int key_idx;
    char value[MAX_VALUE_LEN];
    struct WriteEntry *next;
} WriteEntry;

typedef struct Transaction {
    txnid_t id;
    ts_t start_ts;
    txn_status_t status;
    WriteEntry *writes;
    pthread_mutex_t mtx;
} Transaction;

Transaction *txns[MAX_TXNS];
pthread_mutex_t txns_mtx = PTHREAD_MUTEX_INITIALIZER;
static ts_t global_ts = 1;
static pthread_mutex_t ts_mtx = PTHREAD_MUTEX_INITIALIZER;
static txnid_t next_txn_id = 1;

Transaction *create_txn() {
    pthread_mutex_lock(&txns_mtx);
    if (next_txn_id >= MAX_TXNS) { pthread_mutex_unlock(&txns_mtx); return NULL; }
    Transaction *t = calloc(1, sizeof(Transaction));
    t->id = next_txn_id++;
    pthread_mutex_init(&t->mtx, NULL);
    t->writes = NULL;
    t->status = TXN_ACTIVE;
    pthread_mutex_lock(&ts_mtx);
    t->start_ts = global_ts++;
    pthread_mutex_unlock(&ts_mtx);
    txns[t->id] = t;
    pthread_mutex_unlock(&txns_mtx);
    return t;
}

Transaction *get_txn(txnid_t id) {
    if (id<=0 || id>=MAX_TXNS) return NULL;
    return txns[id];
}

WriteEntry *find_local_write(Transaction *t, int key_idx) {
    for (WriteEntry *w=t->writes; w; w=w->next) if (w->key_idx==key_idx) return w;
    return NULL;
}

char *txn_read(Transaction *t, const char *key) {
    if (!t || t->status != TXN_ACTIVE) return NULL;
    int idx = get_or_create_key(key);

    WriteEntry *w = find_local_write(t, idx);
    if (w) {
        return strdup(w->value);
    }

   
    KeyEntry *ke = &store[idx];
    pthread_mutex_lock(&ke->mtx);
    Version *v = ke->versions;
    while (v && v->commit_ts > t->start_ts) v = v->next;
    char *res = NULL;
    if (v) res = strdup(v->value);
    else {
        res = strdup("");
    }
    pthread_mutex_unlock(&ke->mtx);
    return res;
}

int txn_write(Transaction *t, const char *key, const char *value) {
    if (!t) return -1;
    if (t->status != TXN_ACTIVE) return -1;
    int idx = get_or_create_key(key);
    WriteEntry *w = find_local_write(t, idx);
    if (!w) {
        w = malloc(sizeof(WriteEntry));
        w->key_idx = idx;
        w->next = t->writes;
        t->writes = w;
    }
    strncpy(w->value, value, MAX_VALUE_LEN-1);
    w->value[MAX_VALUE_LEN-1] = '\0';
    return 0;
}

static int lock_write_keys_sorted(Transaction *t, int locked_idxs[], int *locked_count) {
    int present[MAX_KEYS] = {0};
    int keys[MAX_KEYS];
    int kcount = 0;
    for (WriteEntry *w = t->writes; w; w = w->next) {
        if (!present[w->key_idx]) {
            present[w->key_idx] = 1;
            keys[kcount++] = w->key_idx;
        }
    }
    if (kcount == 0) { *locked_count = 0; return 0; }
    for (int i=0;i<kcount;i++) {
        for (int j=i+1;j<kcount;j++) {
            if (keys[j] < keys[i]) {
                int tmp = keys[i]; keys[i]=keys[j]; keys[j]=tmp;
            }
        }
    }
    for (int i=0;i<kcount;i++) {
        pthread_mutex_lock(&store[keys[i]].mtx);
        locked_idxs[i] = keys[i];
    }
    *locked_count = kcount;
    return 0;
}

static void unlock_locked_idxs(int locked_idxs[], int locked_count) {
    for (int i=0;i<locked_count;i++) {
        pthread_mutex_unlock(&store[locked_idxs[i]].mtx);
    }
}

int txn_commit(Transaction *t) {
    if (!t) return -1;
    pthread_mutex_lock(&t->mtx);
    if (t->status != TXN_ACTIVE) {
        pthread_mutex_unlock(&t->mtx);
        return -1;
    }

    if (!t->writes) {
        pthread_mutex_lock(&ts_mtx);

        ts_t my_commit_ts = global_ts++;
        (void)my_commit_ts;
        pthread_mutex_unlock(&ts_mtx);
        t->status = TXN_COMMITTED;
        pthread_mutex_unlock(&t->mtx);
        printf("[TXN %d] Committed (read-only).\n", t->id);
        return 0;
    }

    int locked_idxs[MAX_KEYS];
    int locked_count = 0;
    lock_write_keys_sorted(t, locked_idxs, &locked_count);

    int conflict = 0;
    for (WriteEntry *w = t->writes; w; w = w->next) {
        KeyEntry *ke = &store[w->key_idx];
        Version *head = ke->versions;
        if (head && head->commit_ts > t->start_ts) {
            conflict = 1;
            break;
        }
    }

    if (conflict) {
        unlock_locked_idxs(locked_idxs, locked_count);
        
        WriteEntry *we = t->writes;
        while (we) { WriteEntry *n=we->next; free(we); we=n; }
        t->writes = NULL;
        t->status = TXN_ABORTED;
        pthread_mutex_unlock(&t->mtx);
        printf("[TXN %d] Aborted due to write-write conflict (first-committer-wins).\n", t->id);
        return -1;
    }

    pthread_mutex_lock(&ts_mtx);
    ts_t commit_ts = global_ts++;
    pthread_mutex_unlock(&ts_mtx);

    for (WriteEntry *w = t->writes; w; w = w->next) {
        KeyEntry *ke = &store[w->key_idx];
        Version *v = malloc(sizeof(Version));
        strncpy(v->value, w->value, MAX_VALUE_LEN-1);
        v->value[MAX_VALUE_LEN-1] = '\0';
        v->commit_ts = commit_ts;
        v->next = ke->versions;
        ke->versions = v;
    }

    WriteEntry *we = t->writes;
    while (we) { WriteEntry *n=we->next; free(we); we=n; }
    t->writes = NULL;

    unlock_locked_idxs(locked_idxs, locked_count);

    t->status = TXN_COMMITTED;
    pthread_mutex_unlock(&t->mtx);
    printf("[TXN %d] Committed (commit_ts=%lu).\n", t->id, commit_ts);
    return 0;
}

void txn_explicit_abort(Transaction *t) {
    if (!t) return;
    pthread_mutex_lock(&t->mtx);
    if (t->status != TXN_ACTIVE) { pthread_mutex_unlock(&t->mtx); return; }

    WriteEntry *we = t->writes;
    while (we) { WriteEntry *n=we->next; free(we); we=n; }
    t->writes = NULL;
    t->status = TXN_ABORTED;
    pthread_mutex_unlock(&t->mtx);
    printf("[TXN %d] Explicitly aborted.\n", t->id);
}

void *txn_demo_reader_writer(void *arg) {
    Transaction *t = create_txn();
    printf("[TXN %d] Started (ts=%lu) - will read A, sleep, then write A\n", t->id, t->start_ts);
    char *before = txn_read(t, "A");
    printf("[TXN %d] Read A before = '%s'\n", t->id, before ? before : "(null)");
    free(before);
    sleep(2);
    if (txn_write(t, "A", "T1_A") != 0) { printf("[TXN %d] Aborted while writing A\n", t->id); return NULL; }
    if (txn_commit(t) != 0) { printf("[TXN %d] Commit failed\n", t->id); }
    return NULL;
}

void *txn_demo_competing_writer(void *arg) {
    Transaction *t = create_txn();
    printf("[TXN %d] Started (ts=%lu) - will sleep then write A\n", t->id, t->start_ts);
    sleep(1); 
    if (txn_write(t, "A", "T2_A") != 0) { printf("[TXN %d] Aborted while writing A\n", t->id); return NULL; }
    if (txn_commit(t) != 0) { printf("[TXN %d] Commit failed\n", t->id); }
    return NULL;
}

int main() {
    memset(store, 0, sizeof(store));
    memset(txns, 0, sizeof(txns));
    printf("MVCC demo: snapshot reads + first-committer-wins on write-write conflict.\n");

    pthread_t th1, th2;
    pthread_create(&th1, NULL, txn_demo_reader_writer, NULL);
    usleep(200000); 
    pthread_create(&th2, NULL, txn_demo_competing_writer, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    printf("Final store state (versions newest->oldest):\n");
    pthread_mutex_lock(&store_mtx);
    for (int i=0;i<MAX_KEYS;i++) {
        if (store[i].used) {
            printf("Key=%s\n", store[i].key);
            pthread_mutex_lock(&store[i].mtx);
            Version *v = store[i].versions;
            while (v) {
                printf("  commit_ts=%lu value=%s\n", v->commit_ts, v->value);
                v = v->next;
            }
            pthread_mutex_unlock(&store[i].mtx);
        }
    }
    pthread_mutex_unlock(&store_mtx);
    return 0;
}