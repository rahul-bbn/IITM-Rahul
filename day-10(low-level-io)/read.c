#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

int main()
{
    char buffer[128];

    int fd = open("example.txt", O_RDONLY);

    if(fd == -1)
    {
        perror("open");
        return 1;
    }

    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    
    if(bytes_read == -1)
    {
        perror("read");
        close(fd);
        return 1;
    }

    buffer[bytes_read] = '\0';
    printf("Read %zd bytes : %s \n", bytes_read, buffer);

    close(fd);

    return 0;
}