#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include<errno.h>
#include<fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    fd_set readfds;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and connect
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("Invalid address");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Connection failed");
        return -1;
    }

    printf("Connected to server! Type messages:\n");

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds); // Keyboard input
        FD_SET(sock, &readfds);         // Server message

        int max_sd = sock;
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0)
        {
            perror("select error");
            continue;
        }

        // If user types a message
        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            if (fgets(buffer, BUFFER_SIZE, stdin) != NULL)
            {
                send(sock, buffer, strlen(buffer), 0);
            }
        }

        // If server sends a message
        if (FD_ISSET(sock, &readfds))
        {
            int valread = read(sock, buffer, BUFFER_SIZE);
            if (valread == 0)
            {
                printf("Server disconnected.\n");
                break;
            }
            buffer[valread] = '\0';
            printf("message:  %s", buffer);
        }
    }

    close(sock);
    return 0;
}
