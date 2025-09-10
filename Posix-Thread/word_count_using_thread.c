#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>

#define MAX_THREADS 4
#define BUF_SIZE 8192

typedef struct {
    int fd;
    off_t start;
    off_t end;
    size_t word_count;
} ThreadData;

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

// Check if a character is a delimiter
int is_delim(char c) {
    return isspace((unsigned char)c);
}

// Thread function: count words in file segment
void *count_words(void *arg) {
    ThreadData *tdata = (ThreadData *)arg;
    char buf[BUF_SIZE];
    off_t pos = tdata->start;
    size_t count = 0;
    int in_word = 0;

    while (pos < tdata->end) {
        ssize_t to_read = BUF_SIZE;
        if (tdata->end - pos < to_read)
            to_read = tdata->end - pos;

        ssize_t n = pread(tdata->fd, buf, to_read, pos);
        if (n <= 0) break;

        for (ssize_t i = 0; i < n; i++) {
            if (is_delim(buf[i])) {
                if (in_word) {
                    count++;
                    in_word = 0;
                }
            } else {
                in_word = 1;
            }
        }
        pos += n;
    }

    if (in_word) count++;

    tdata->word_count = count;

    pthread_mutex_lock(&print_mutex);
    printf("Thread [%lld - %lld]: %zu words\n", (long long)tdata->start, (long long)tdata->end, count);
    pthread_mutex_unlock(&print_mutex);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }

    struct stat st;
    if (fstat(fd, &st) < 0) { perror("fstat"); return 1; }
    if (st.st_size == 0) { fprintf(stderr, "File is empty\n"); return 0; }

    pthread_t threads[MAX_THREADS];
    ThreadData tdata[MAX_THREADS];
    off_t chunk_size = st.st_size / MAX_THREADS;

    for (int i = 0; i < MAX_THREADS; i++) {
        tdata[i].fd = fd;
        tdata[i].start = i * chunk_size;
        tdata[i].end = (i == MAX_THREADS - 1) ? st.st_size : (i + 1) * chunk_size;

        // Adjust start to avoid splitting a word
        if (i > 0) {
            char c;
            while (pread(fd, &c, 1, tdata[i].start) == 1 && !is_delim(c))
                tdata[i].start++;
        }

        pthread_create(&threads[i], NULL, count_words, &tdata[i]);
    }

    size_t total_words = 0;
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total_words += tdata[i].word_count;
    }

    printf("Total words: %zu\n", total_words);

    pthread_mutex_destroy(&print_mutex);
    close(fd);
    return 0;
}
