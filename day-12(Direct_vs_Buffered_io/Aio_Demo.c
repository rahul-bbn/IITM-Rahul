#include <aio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

int main() {
    int fd = open("data.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    struct aiocb cb;
    memset(&cb, 0, sizeof(cb));

    cb.aio_fildes = fd;
    cb.aio_buf = malloc(256);
    cb.aio_nbytes = 256;
    cb.aio_offset = 0;

    if (!cb.aio_buf) {
        perror("malloc");
        close(fd);
        return 1;
    }

    // Record start time
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Issue asynchronous read
    if (aio_read(&cb) == -1) {
        perror("aio_read");
        free((void*)cb.aio_buf);
        close(fd);
        return 1;
    }

    // Wait until request finishes
    while (aio_error(&cb) == EINPROGRESS) {
        // Simulate doing other work
    }

    // Record end time
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Get result
    int ret = aio_return(&cb);
    if (ret > 0) {
        printf("Read %d bytes: %.*s\n", ret, ret, (char*)cb.aio_buf);
    } else {
        printf("Read failed\n");
    }

    // Calculate elapsed time in milliseconds
    double elapsed = (end.tv_sec - start.tv_sec) * 1000.0 +
                     (end.tv_nsec - start.tv_nsec) / 1000000.0;
    printf("Elapsed time: %.3f ms\n", elapsed);

    free((void*)cb.aio_buf);
    close(fd);
    return 0;
}