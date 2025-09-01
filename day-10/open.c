#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

int main()
{
    int fd = open("example.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if(fd == -1)
    {
        perror("open");
        return 1;
    }

    printf("Opened the file with %d \n", fd);

    close(fd);
    return 0;
}