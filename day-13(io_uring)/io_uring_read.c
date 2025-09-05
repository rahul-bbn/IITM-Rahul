#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <liburing.h>

#define BUF_SIZE 1024

int main() {
    struct io_uring ring;
    struct io_uring_sqe *sqe;
    struct io_uring_cqe *cqe;
    int fd;
    char buffer[BUF_SIZE];

    // 1. Open file
    fd = open("data.txt", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // 2. Initialize io_uring
    if (io_uring_queue_init(8, &ring, 0) < 0) {
        perror("io_uring_queue_init");
        close(fd);
        return 1;
    }

    // 3. Get a submission queue entry (SQE)
    sqe = io_uring_get_sqe(&ring);

    // 4. Prepare a read request
    io_uring_prep_read(sqe, fd, buffer, BUF_SIZE, 0);

    // 5. Submit the request
    io_uring_submit(&ring);

    // 6. Wait for completion (CQE)
    io_uring_wait_cqe(&ring, &cqe);

    if (cqe->res < 0) {
        fprintf(stderr, "Read failed: %s\n", strerror(-cqe->res));
    } else {
        printf("Read %d bytes:\n", cqe->res);
        write(1, buffer, cqe->res); // print to stdout
        printf("\n");
    }

    // 7. Mark CQE as seen
    io_uring_cqe_seen(&ring, cqe);

    // 8. Cleanup
    io_uring_queue_exit(&ring);
    close(fd);

    return 0;
}
