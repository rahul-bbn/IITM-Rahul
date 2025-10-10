#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    char buff[100];
    int fd;
    int maxfd;
    // fd_set: used to store file descriptors to monitor for readability
    fd_set readfds;
    int activity;

    fd = open("test.txt",O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        exit(1);
    }
    maxfd = (fd > STDIN_FILENO ? fd : STDIN_FILENO) + 1;

    printf("Waiting for input from keyboard or file 'test.txt'...\n");

    while (1)
    {
        // Clear the fd_set before each select() call
        FD_ZERO(&readfds);

        // Add stdin (keyboard) to the set
        FD_SET(STDIN_FILENO, &readfds);

        // Add file descriptor to the set
        FD_SET(fd, &readfds);

        // Block until at least one fd is ready for reading
        activity = select(maxfd, &readfds, NULL, NULL, NULL);

        if (activity < 0)
        {
            perror("select");
            break;
        }
        else if (activity == 0)
        {
            printf("Timeout: No input in 5 seconds.\n");
            continue;
        }
        // Check if stdin has data to read
        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            int n = read(STDIN_FILENO, buff, sizeof(buff) - 1);
            if (n > 0)
            {
                buff[n] = '\0'; // Null-terminate file content
                printf("You typed: %s\n", buff);
            }
        }
        // Check if the file has data to read
        if (FD_ISSET(fd, &readfds))
        {
            int n = read(fd, buff, sizeof(buff));
            if (n > 0)
            {
                buff[n] = '\0'; // Null-terminate file content
                printf("File content: %s\n", buff);
            }
        }
    }
    close(fd);
    return 0;
}