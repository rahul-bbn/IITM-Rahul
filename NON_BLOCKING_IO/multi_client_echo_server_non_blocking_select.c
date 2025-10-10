#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>


#define PORT 8080
#define MAX_CLIENTS FD_SETSIZE
#define BUFFER_SIZE 1024

void set_nonblocking(int fd)
{
    
}

int main()
{
    int server_fd, new_socket, client_socket[MAX_CLIENTS];
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    fd_set readfds;
    int max_sd, activity, valread;

    for (int i = 0; i < MAX_CLIENTS; i++)
        client_socket[i] = 0;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int flags = fcntl(server_fd, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl F_GETFL");
        exit(1);
    }
    if (fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl F_SETFL");
        exit(1);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Server started on port %d\n", PORT);

    if (listen(server_fd, 10) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        FD_ZERO(&readfds);

        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            int sd = client_socket[i];
            if (sd > 0)
                FD_SET(sd, &readfds);
            if (sd > max_sd)
                max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0 && errno != EINTR)
        {
            perror("select error");
        }

        if (FD_ISSET(server_fd, &readfds))
        {
            new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            if (new_socket < 0)
            {
                if (errno != EWOULDBLOCK)
                    perror("accept");
            }
            else
            {
                printf("New connection: fd=%d\n", new_socket);
                set_nonblocking(new_socket);

                for (int i = 0; i < MAX_CLIENTS; i++)
                {
                    if (client_socket[i] == 0)
                    {
                        client_socket[i] = new_socket;
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            int sd = client_socket[i];
            if (sd > 0 && FD_ISSET(sd, &readfds))
            {
                memset(buffer, 0, BUFFER_SIZE);
                valread = read(sd, buffer, BUFFER_SIZE);
                if (valread == 0)
                {
                    printf("Client disconnected: fd=%d\n", sd);
                    close(sd);
                    client_socket[i] = 0;
                }
                else if (valread > 0)
                {
                    printf("Received from fd=%d: %s", sd, buffer);
                    send(sd, buffer, valread, 0);
                }
                else if (errno != EWOULDBLOCK)
                {
                    perror("read");
                    close(sd);
                    client_socket[i] = 0;
                }
            }
        }
    }

    close(server_fd);
    return 0;
}