#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUF_SIZE 100

// Define FIFOs (reversed compared to chat1)
char *read_fifo  = "/tmp/fifo1";  // chat2 reads from fifo1
char *write_fifo = "/tmp/fifo2";  // chat2 writes to fifo2

void* read_messages(void* arg) {
    char buf[BUF_SIZE];
    int fd;

    while (1) {
        fd = open(read_fifo, O_RDONLY);
        if (fd < 0) {
            perror("open read_fifo failed");
            exit(1);
        }

        memset(buf, 0, BUF_SIZE);
        read(fd, buf, BUF_SIZE);

        printf("\nUser1: %sYou: ", buf);
        fflush(stdout);
        close(fd);
    }
    return NULL;
}

int main() {
    pthread_t tid;
    char buf[BUF_SIZE];
    int fd;

    mkfifo(write_fifo, 0666);
    mkfifo(read_fifo, 0666);

    pthread_create(&tid, NULL, read_messages, NULL);

    while (1) {
        printf("You: ");
        fgets(buf, BUF_SIZE, stdin);

        fd = open(write_fifo, O_WRONLY);
        if (fd < 0) {
            perror("open write_fifo failed");
            exit(1);
        }

        write(fd, buf, strlen(buf) + 1);
        close(fd);
    }

    return 0;
}