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
    struct Version *next;      // older version
} Version;

typedef struct KeyLock {
    pthread_mutex_t mtx;
    pthread_cond_t  cv;
    txnid_t owner_txn;
} KeyLock;

typedef struct KeyEntry {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
    Version *versions;
    KeyLock lock;
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
            store[idx].value[0] = '\0';
            pthread_mutex_init(&store[idx].lock.mtx, NULL);
            pthread_cond_init(&store[idx].lock.cv, NULL);
            store[idx].lock.owner_txn = 0;
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
    int held_keys[MAX_KEYS];
    pthread_mutex_t mtx;
} Transaction;

Transaction *txns[MAX_TXNS];
pthread_mutex_t txns_mtx = PTHREAD_MUTEX_INITIALIZER;
static ts_t global_ts = 1;
static txnid_t next_txn_id = 1;

int wait_for[MAX_TXNS][MAX_TXNS];
pthread_mutex_t wf_mtx = PTHREAD_MUTEX_INITIALIZER;

Transaction *create_txn() {
    pthread_mutex_lock(&txns_mtx);
    if (next_txn_id >= MAX_TXNS) { pthread_mutex_unlock(&txns_mtx); return NULL; }
    Transaction *t = calloc(1, sizeof(Transaction));
    t->id = next_txn_id++;
    t->start_ts = global_ts++;
    t->status = TXN_ACTIVE;
    t->writes = NULL;
    pthread_mutex_init(&t->mtx, NULL);
    for (int i=0;i<MAX_KEYS;i++) t->held_keys[i]=0;
    txns[t->id] = t;
    pthread_mutex_unlock(&txns_mtx);
    return t;
}

Transaction *get_txn(txnid_t id) {
    if (id<=0 || id>=MAX_TXNS) return NULL;
    return txns[id];
}

void add_wait_edge(int waiter, int owner) {
    if (waiter<=0 || owner<=0) return;
    pthread_mutex_lock(&wf_mtx);
    wait_for[waiter][owner] = 1;
    pthread_mutex_unlock(&wf_mtx);
}

void remove_wait_edge(int waiter, int owner) {
    if (waiter<=0 || owner<=0) return;
    pthread_mutex_lock(&wf_mtx);
    wait_for[waiter][owner] = 0;
    pthread_mutex_unlock(&wf_mtx);
}

int detect_cycle_and_pick_victim(int *victim_out) {
    pthread_mutex_lock(&wf_mtx);
    int n = next_txn_id;
    int visited[MAX_TXNS] = {0}, recstack[MAX_TXNS] = {0};
    int cycle_found = 0;
    int cycle_nodes[MAX_TXNS];
    int cycle_count = 0;
    int stack[MAX_TXNS], top=0;
    int path[MAX_TXNS], path_len=0;
    int winner_victim = -1;
    for (int s = 1; s < n; ++s) {
        if (!txns[s]) continue;
        if (visited[s]) continue;
        int st_stack[MAX_TXNS];
        int edge_idx[MAX_TXNS];
        int sp=0;
        st_stack[sp]=s; edge_idx[sp]=0; sp++;
        while (sp>0) {
            int v = st_stack[sp-1];
            if (!visited[v]) { visited[v]=1; recstack[v]=1; path[path_len++]=v; }
            int found_next = 0;
            for (int u=edge_idx[sp-1]; u < n; ++u) {
                edge_idx[sp-1] = u+1;
                if (wait_for[v][u]) {
                    if (!visited[u]) {
                        st_stack[sp]=u; edge_idx[sp]=0; sp++; found_next=1; break;
                    } else if (recstack[u]) {
                        cycle_count = 0;
                        int picked_victim = -1;
                        ts_t picked_ts = 0;
                        for (int k=0;k<path_len;k++) {
                            int node = path[k];
                            if (node == u) {
                                for (int j=k; j<path_len; ++j) {
                                    int cyc_node = path[j];
                                    cycle_nodes[cycle_count++] = cyc_node;
                                    Transaction *t = txns[cyc_node];
                                    if (t) {
                                        if (picked_victim == -1 || t->start_ts > picked_ts) {
                                            picked_victim = cyc_node;
                                            picked_ts = t->start_ts;
                                        }
                                    }
                                }
                                break;
                            }
                        }
                        if (picked_victim == -1 && txns[v]) { picked_victim = v; picked_ts = txns[v]->start_ts; }
                        if (picked_victim != -1) {
                            if (winner_victim == -1 || txns[picked_victim]->start_ts > txns[winner_victim]->start_ts) {
                                winner_victim = picked_victim;
                            }
                            cycle_found = 1;
                        }
                    }
                }
            }
            if (!found_next) {
                recstack[v]=0;
                st_stack[sp-1]=0;
                sp--;
                path_len--;
            }
        }
    }
    pthread_mutex_unlock(&wf_mtx);
    if (cycle_found) {
        *victim_out = winner_victim;
        return 1;
    }
    return 0;
}

void release_all_locks(Transaction *t) {
    if (!t) return;
    for (int i=0;i<MAX_KEYS;i++) {
        if (t->held_keys[i]) {
            KeyLock *lk = &store[i].lock;
            pthread_mutex_lock(&lk->mtx);
            if (lk->owner_txn == t->id) {
                lk->owner_txn = 0;
                pthread_cond_broadcast(&lk->cv);
            }
            pthread_mutex_unlock(&lk->mtx);
            t->held_keys[i]=0;
        }
    }
}

void abort_txn(Transaction *t) {
    if (!t) return;
    pthread_mutex_lock(&t->mtx);
    if (t->status != TXN_ACTIVE) {
        pthread_mutex_unlock(&t->mtx);
        return;
    }
    t->status = TXN_ABORTED;
    pthread_mutex_lock(&wf_mtx);
    for (int i=1;i<next_txn_id;i++) wait_for[i][t->id] = 0;
    for (int j=1;j<next_txn_id;j++) wait_for[t->id][j] = 0;
    pthread_mutex_unlock(&wf_mtx);
    WriteEntry *w = t->writes;
    while (w) { WriteEntry *n=w->next; free(w); w=n; }
    t->writes = NULL;
    release_all_locks(t);
    pthread_mutex_unlock(&t->mtx);
    printf("[TXN %d] Aborted (victim or explicit).\n", t->id);
}

int acquire_key_lock(Transaction *t, int key_idx) {
    KeyLock *lk = &store[key_idx].lock;
    pthread_mutex_lock(&lk->mtx);
    while (1) {
        if (t->status != TXN_ACTIVE) {
            pthread_mutex_unlock(&lk->mtx);
            return -1;
        }
        if (lk->owner_txn == 0) {
            lk->owner_txn = t->id;
            t->held_keys[key_idx] = 1;
            pthread_mutex_unlock(&lk->mtx);
            return 0;
        } else if (lk->owner_txn == t->id) {
            pthread_mutex_unlock(&lk->mtx);
            return 0;
        } else {
            txnid_t owner = lk->owner_txn;
            add_wait_edge(t->id, owner);
            int victim = -1;
            if (detect_cycle_and_pick_victim(&victim)) {
                if (victim == t->id) {
                    remove_wait_edge(t->id, owner);
                    pthread_mutex_unlock(&lk->mtx);
                    abort_txn(t);
                    return -1;
                } else {
                    Transaction *vt = get_txn(victim);
                    if (vt) {
                        printf("[DEADLOCK] Cycle detected. Aborting victim txn %d (youngest in cycle)\n", victim);
                        abort_txn(vt);
                        remove_wait_edge(t->id, owner);
                        continue;
                    }
                }
            }
            while (lk->owner_txn != 0 && lk->owner_txn != t->id && t->status == TXN_ACTIVE) {
                pthread_cond_wait(&lk->cv, &lk->mtx);
            }
            remove_wait_edge(t->id, owner);
        }
    }
}

void release_key_lock(Transaction *t, int key_idx) {
    KeyLock *lk = &store[key_idx].lock;
    pthread_mutex_lock(&lk->mtx);
    if (lk->owner_txn == t->id) {
        lk->owner_txn = 0;
        pthread_cond_broadcast(&lk->cv);
    }
    pthread_mutex_unlock(&lk->mtx);
    t->held_keys[key_idx] = 0;
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
    pthread_mutex_lock(&store_mtx);
    char tmp[MAX_VALUE_LEN];
    strncpy(tmp, store[idx].value, MAX_VALUE_LEN-1);
    tmp[MAX_VALUE_LEN-1]='\0';
    pthread_mutex_unlock(&store_mtx);
    return strdup(tmp);
}

int txn_write(Transaction *t, const char *key, const char *value) {
    if (!t) return -1;
    if (t->status != TXN_ACTIVE) return -1;
    int idx = get_or_create_key(key);
    if (acquire_key_lock(t, idx) != 0) {
        return -1;
    }
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

int txn_commit(Transaction *t) {
    if (!t) return -1;
    pthread_mutex_lock(&t->mtx);
    if (t->status != TXN_ACTIVE) {
        pthread_mutex_unlock(&t->mtx);
        return -1;
    }
    for (WriteEntry *w = t->writes; w; w = w->next) {
        pthread_mutex_lock(&store_mtx);
        strncpy(store[w->key_idx].value, w->value, MAX_VALUE_LEN-1);
        store[w->key_idx].value[MAX_VALUE_LEN-1] = '\0';
        pthread_mutex_unlock(&store_mtx);
    }
    WriteEntry *we = t->writes;
    while (we) { WriteEntry *n=we->next; free(we); we=n; }
    t->writes = NULL;
    release_all_locks(t);
    t->status = TXN_COMMITTED;
    pthread_mutex_unlock(&t->mtx);
    printf("[TXN %d] Committed.\n", t->id);
    return 0;
}

void txn_explicit_abort(Transaction *t) {
    abort_txn(t);
}

void *txn_func_ab_deadlock1(void *arg) {
    Transaction *t = create_txn();
    printf("[TXN %d] Started (ts=%lu) - will write key A then B\n", t->id, t->start_ts);
    if (txn_write(t, "A", "T1_A") != 0) { printf("[TXN %d] Aborted while writing A\n", t->id); return NULL; }
    sleep(1);
    if (txn_write(t, "B", "T1_B") != 0) { printf("[TXN %d] Aborted while writing B\n", t->id); return NULL; }
    txn_commit(t);
    return NULL;
}

void *txn_func_ab_deadlock2(void *arg) {
    Transaction *t = create_txn();
    printf("[TXN %d] Started (ts=%lu) - will write key B then A\n", t->id, t->start_ts);
    if (txn_write(t, "B", "T2_B") != 0) { printf("[TXN %d] Aborted while writing B\n", t->id); return NULL; }
    sleep(1);
    if (txn_write(t, "A", "T2_A") != 0) { printf("[TXN %d] Aborted while writing A\n", t->id); return NULL; }
    txn_commit(t);
    return NULL;
}

int main() {
    memset(store, 0, sizeof(store));
    memset(txns, 0, sizeof(txns));
    memset(wait_for, 0, sizeof(wait_for));
    printf("Starting demo: create deadlock between two txns; deadlock detector should abort the youngest.\n");

    pthread_t th1, th2;
    pthread_create(&th1, NULL, txn_func_ab_deadlock1, NULL);
    usleep(200000);
    pthread_create(&th2, NULL, txn_func_ab_deadlock2, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    printf("Final store state:\n");
    pthread_mutex_lock(&store_mtx);
    for (int i=0;i<MAX_KEYS;i++) {
        if (store[i].used) {
            printf("Key=%s Value=%s\n", store[i].key, store[i].value);
        }
    }
    pthread_mutex_unlock(&store_mtx);
    return 0;
}