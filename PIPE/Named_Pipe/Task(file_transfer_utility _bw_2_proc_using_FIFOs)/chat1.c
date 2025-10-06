#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUF_SIZE 100

char *read_fifo  = "/tmp/fifo2";  // chat1 reads from fifo2
char *write_fifo = "/tmp/fifo1";  // chat1 writes to fifo1

// Thread function for reading messages
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

        printf("\nUser2: %sYou: ", buf);
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

    // Start the reader thread
    pthread_create(&tid, NULL, read_messages, NULL);

    // Loop to send messages
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