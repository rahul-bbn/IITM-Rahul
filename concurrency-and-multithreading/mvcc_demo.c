// mvcc -> multi-version concurrency control

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdint.h>
#include<unistd.h>

typedef struct Version {
    int value;
    unsigned long begin_ts;
    unsigned long end_ts;
    struct Version *next; //newest first, singly linked
}Version;

typedef struct {
    pthread_mutex_t mutex;
    Version *head; //newest version at the head
} Row;

static Row row;
static pthread_mutex_t tx_mutex = PTHREAD_MUTEX_INITIALIZER;
static unsigned long global_ts = 1UL;

//next_ts
static unsigned long next_ts(void)
{
    pthread_mutex_lock(&tx_mutex);
    unsigned long t = ++global_ts;
    pthread_mutex_unlock(&tx_mutex);
    return t;
}

//begin_tx
static unsigned long begin_tx(void)
{
    pthread_mutex_lock(&tx_mutex);
    unsigned long t = global_ts; //read current clock
    pthread_mutex_unlock(&tx_mutex);
    return t;
}

//row_init
static void row_init(Row *row, int initial_value)
{
    pthread_mutex_init(&row->mutex, NULL);

    Version *v = (Version*)malloc(sizeof(Version));

    v->value = initial_value;
    v->begin_ts = 1UL;
    v->end_ts = UINT64_MAX;
    v->next = NULL;

    row->head = v;
}

//mvcc_read
static int mvcc_read(Row *row, unsigned long start_ts)
{
    pthread_mutex_lock(&row->mutex);
    Version *v = row->head;

    while(v)
    {
        if(v->begin_ts <= start_ts && start_ts < v->end_ts)
        {
            int val = v->value;
            pthread_mutex_unlock(&row->mutex);
            return val;
        }
        v = v->next;
    }
    pthread_mutex_unlock(&row->mutex);
    return -1; //if not found
}

//mvcc_write_commit - stage & commit a new version
static void mvcc_write_commit(Row *row, int new_value)
{
    unsigned long commit_timestamp = next_ts();

    pthread_mutex_lock(&row->mutex);
    Version *old = row->head;

    if(old && old->end_ts == UINT64_MAX)
    {
        old->end_ts = commit_timestamp;
    }

    Version *new = (Version*)malloc(sizeof(Version));

    new->value = new_value;
    new->begin_ts = commit_timestamp; //new version is visible to snapshots whose are >= commit_timestamp
    new->end_ts = UINT64_MAX;
    new->next = row->head;
    row->head = new;

    pthread_mutex_unlock(&row->mutex);
}

//reader1
static void *reader_one(void *arg)
{
    (void)arg;
    unsigned long s = begin_tx(); //snapshot before the writer commits
    printf("[Reader One] start_ts = %lu \n", s);

    for(int iterator = 0; iterator < 5; ++iterator)
    {
        int read_value = mvcc_read(&row, s);
        printf("[Reader One] read = %d (snapshot) \n", read_value);
        usleep(150000);
    }

    return NULL;
}

//reader2
static void *reader_two(void *arg)
{
    (void)arg;

    usleep(500000); //read later, after the writer

    unsigned long s = begin_tx(); 

    printf("[Reader Two] start_ts = %lu \n", s);

    int read_value = mvcc_read(&row, s);
    printf("[Reader Two] read = %d (latest snapshot) \n", read_value);

    usleep(150000);

    return NULL;
}

//writer
static void *writer(void *arg)
{
    (void)arg;
    usleep(250000); //give reader one enough time to start

    printf("[Writer] Committing new value = 200 \n");
    mvcc_write_commit(&row, 200);
    printf("[Writer] Committed \n");

    return NULL;
}

//main
int main (void)
{
    row_init(&row, 100);

    pthread_t reader1, reader2, writer_w;

    pthread_create(&reader1, NULL, reader_one, NULL);
    pthread_create(&writer_w, NULL, writer, NULL);
    pthread_create(&reader2, NULL, reader_two, NULL);

    pthread_join(reader1, NULL);
    pthread_join(writer_w, NULL);
    pthread_join(reader2, NULL);

    return 0;
}