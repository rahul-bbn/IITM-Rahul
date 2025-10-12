#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include<errno.h>
#include<fcntl.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUF_SIZE 1024

int main()
{
    int server_fd, new_socket, client_socket[MAX_CLIENTS];
    struct sockaddr_in address;
    int max_sd, sd, activity, valread;
    char buffer[BUF_SIZE];
    fd_set readfds;

    // Initialize all client sockets to 0 (unused)
    for (int i = 0; i < MAX_CLIENTS; i++)
        client_socket[i] = 0;

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set SO_REUSEADDR to reuse the port quickly
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Bind to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening
    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Chat server started on port %d\n", PORT);

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        // Add client sockets to the set
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_socket[i];
            if (sd > 0)
                FD_SET(sd, &readfds);
            if (sd > max_sd)
                max_sd = sd;
        }

        // Wait for activity
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR))
        {
            perror("select error");
        }

        // Check for new connection
        if (FD_ISSET(server_fd, &readfds))
        {
            int addrlen = sizeof(address);
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("New connection: socket %d, IP %s, port %d\n",
                   new_socket,
                   inet_ntoa(address.sin_addr),
                   ntohs(address.sin_port));

            // Add new socket to client list
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    break;
                }
            }

            // Send welcome message
            char *msg = "Welcome to the chat!\n";
            send(new_socket, msg, strlen(msg), 0);
        }

        // Handle I/O for existing clients
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET(sd, &readfds))
            {
                memset(buffer, 0, BUF_SIZE);
                valread = read(sd, buffer, BUF_SIZE);

                if (valread == 0)
                {
                    // Client disconnected
                    getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&(int){sizeof(address)});
                    printf("Client disconnected: IP %s, port %d\n",
                           inet_ntoa(address.sin_addr),
                           ntohs(address.sin_port));

                    close(sd);
                    client_socket[i] = 0;
                }
                else
                {
                    // Broadcast message to all other clients
                    buffer[valread] = '\0';
                    for (int j = 0; j < MAX_CLIENTS; j++)
                    {
                        if (client_socket[j] != 0 && client_socket[j] != sd)
                        {
                            send(client_socket[j], buffer, strlen(buffer), 0);
                        }
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
