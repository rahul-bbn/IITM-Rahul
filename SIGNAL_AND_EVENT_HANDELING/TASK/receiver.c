#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_sigusr1(int sig) {
    printf("Received SIGUSR1 (%d)\n", sig);
}

void handle_sigusr2(int sig) {
    printf("Received SIGUSR2 (%d)\n", sig);
}

void handle_sigterm(int sig) {
    printf("Received SIGTERM (%d), exiting...\n", sig);
    exit(0);
}

int main() {
    printf("Receiver PID: %d\n", getpid());

    // Register signal handlers
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);
    signal(SIGTERM, handle_sigterm);

    while (1) {
        pause(); // Sleep until a signal is received
    }

    return 0;
}