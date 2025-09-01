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

    const char *msg = "Hello world (added by write()) \n";
    ssize_t total_written = 0;
    ssize_t length = strlen(msg);

    while (total_written < length)
    {
        ssize_t bytes_written = write(fd, msg + total_written, length - total_written); 

        if(bytes_written == -1)
        {
            perror("write");
            close(fd);

            return 1;
        }

        total_written += bytes_written;
    }

    printf("Wrote %zd bytes \n", total_written);

    close(fd);
    
    return 0;
}