#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int main()
{
    const char *fifopath = "/tmp/myfifo"; // Linux-native path

    // Create FIFO if it doesn't exist
    if (mkfifo(fifopath, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("failed fifo");
            return 1;
        }
    }

    int fd = open(fifopath, O_WRONLY);
    if (fd == -1)
    {
        perror("open failed");
        return 1;
    }

    char msg[] = "Hello from Writer via FIFO!";
    write(fd, msg, sizeof(msg));
    close(fd);

    printf("Writer sent: %s\n", msg);
    return 0;
}
