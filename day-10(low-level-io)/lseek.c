#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

int main()
{
    int fd = open("example.txt", O_RDWR, 0644);

    if(fd == -1)
    {
        perror("open");
        return 1;
    }

    if (lseek(fd, 5, SEEK_SET) == -1)
    {
       perror("lseek");
       close(fd);
       return 1; 
    }

    const char *msg = "Inserted text";

    if(write(fd, msg, 6) == -1)
    {
        perror("write");
        close (fd);
        return 1;
    }

    printf("Inserted at offset 5 \n");

    close(fd);

    return 0;
}