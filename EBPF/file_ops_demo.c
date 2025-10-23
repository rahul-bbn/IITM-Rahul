#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/syscall.h>
 
int main() {

    int fd = open("sample.txt", O_RDONLY | O_CREAT);

    if (fd < 0) {

        perror("open");

        return 1;

    }
 
    char buffer[16];

    ssize_t bytes;
 
    for (int i = 0; i < 3; i++) {

        bytes = read(fd, buffer, sizeof(buffer));

        if (bytes <= 0)

            break;

        printf("Read %ld bytes\n", bytes);

        printf("process id: %d\n", getpid());
        printf("TID: %ld\n",syscall(SYS_gettid));

        sleep(1);

    }
    close(fd);

    return 0;
}
 