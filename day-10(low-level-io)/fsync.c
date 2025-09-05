#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>

int main()
{
    int fd = open("example.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if(fd == -1)
    {
        perror("open");
        return 1;
    }

    const char *msg = "Durable file write! \n";

    if(write(fd, msg, strlen(msg)) == -1)
    {
        perror("write");
        close(fd);
        return 1;
    }

    if(fsync(fd) == -1)
    {
       perror("fsync");
       close(fd);
       return 1; 
    }

    printf("Data synced to the disk \n");

    close(fd);

    return 0;
}