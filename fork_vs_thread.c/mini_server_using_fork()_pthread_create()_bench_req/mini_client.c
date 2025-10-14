// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    struct sockaddr_in server_addr;
    int client_socket;
    char *message = "Hello Server";
    char buffer[BUFFER_SIZE];
    struct timeval start, end;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
        error_exit("socket failed");
    
    // // Set SO_REUSEADDR to reuse the port quickly
    // int opt = 1;
    // if (setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    // {
    //     perror("setsockopt");
    //     exit(EXIT_FAILURE);
    // }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        error_exit("connect failed");

    gettimeofday(&start, NULL);
    write(client_socket, message, strlen(message));
    read(client_socket, buffer, BUFFER_SIZE);
    gettimeofday(&end, NULL);

    long latency_us = ((end.tv_sec - start.tv_sec) * 1000000L) +
                      (end.tv_usec - start.tv_usec);
    printf("Response: %sLatency: %ld microseconds\n", buffer, latency_us);

    close(client_socket);
    return 0;
}