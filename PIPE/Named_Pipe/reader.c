#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int main()
{
    const char *fifopath = "/tmp/myfifo"; // same path

    // Create FIFO if it doesn't exist
    if (mkfifo(fifopath, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo failed");
            return 1;
        }
    }

    int fd = open(fifopath, O_RDONLY);
    if (fd < 0)
    {
        perror("open failed");
        return 1;
    }

    char buffer[100];
    read(fd, buffer, sizeof(buffer));
    printf("Reader received: %s\n", buffer);
    close(fd);

    return 0;
}
