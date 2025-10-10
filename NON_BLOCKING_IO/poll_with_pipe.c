#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>


int main(void) {
    const char *FIFO_PATH="/tmp/myfifo";

    if (mkfifo(FIFO_PATH, 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo");
            return 1;
        }
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        sleep(1);
        int wfd = open(FIFO_PATH, O_WRONLY);
        if (wfd < 0) {
            perror("child: open for write");
            exit(1);
        }

        const char *msg = "Hello from writer\n";
        write(wfd, msg, strlen(msg));
        printf("Child: wrote message. Keeping writer open 2s...\n");
        sleep(2);
        close(wfd);
        printf("Child: closed write end and exiting.\n");
        exit(0);
    }

    int rfd = open(FIFO_PATH, O_RDONLY);
    if (rfd < 0) {
        perror("parent: open for read");
        return 1;
    }

    struct pollfd pfd;
    pfd.fd = rfd;
    pfd.events = POLLIN;

    printf("Parent: polling FIFO for POLLIN and POLLHUP...\n");

    char buf[256];
    while (1) {
        int ret = poll(&pfd, 1, -1);
        if (ret < 0) {
            perror("poll");
            break;
        }

        if (pfd.revents & POLLIN) {
            int n = read(rfd, buf, sizeof(buf)-1);
            if (n > 0) {
                buf[n] = '\0';
                printf("Parent: read %d bytes: %s", n, buf);
            } else if (n == 0) {
                printf("Parent: read returned 0 (EOF)\n");
            } else {
                perror("read");
            }
        }

        if (pfd.revents & POLLHUP) {
            printf("Parent: POLLHUP detected (writer closed).\n");
            break;
        }
    }

    close(rfd);
    unlink(FIFO_PATH);          // cleanup FIFO file
    waitpid(pid, NULL, 0);      // reap child
    return 0;
}
