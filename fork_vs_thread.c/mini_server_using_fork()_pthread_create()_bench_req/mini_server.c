// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include "common.h"

int server_socket;

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Worker function (shared by both modes)
void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    free(arg);
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("[Worker] Received: %s\n", buffer);
        write(client_socket, "ACK\n", 4);
    }

    close(client_socket);
    return NULL;
}

// Fork mode: each client handled by child process
void run_fork_mode() {
    printf("[Mode] Process-based workers (fork)\n");

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int *client_socket = malloc(sizeof(int));
        *client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (*client_socket < 0) error_exit("accept failed");

        pid_t pid = fork();
        if (pid == 0) {
            close(server_socket);
            handle_client(client_socket);
            exit(0);
        } else if (pid > 0) {
            close(*client_socket);
        } else {
            error_exit("fork failed");
        }
    }
}

// Thread mode: each client handled by thread
void run_thread_mode() {
    printf("[Mode] Thread-based workers (pthread)\n");

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int *client_socket = malloc(sizeof(int));
        *client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (*client_socket < 0) error_exit("accept failed");

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_socket);
        pthread_detach(tid);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [fork|thread]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) error_exit("socket failed");

    // Set SO_REUSEADDR to reuse the port quickly
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        error_exit("bind failed");

    if (listen(server_socket, MAX_CLIENTS) < 0)
        error_exit("listen failed");

    if (strcmp(argv[1], "fork") == 0)
        run_fork_mode();
    else if (strcmp(argv[1], "thread") == 0)
        run_thread_mode();
    else
        fprintf(stderr, "Invalid mode. Use 'fork' or 'thread'\n");

    close(server_socket);
    return 0;
}