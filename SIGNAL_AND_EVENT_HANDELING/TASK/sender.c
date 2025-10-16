//Implement a signal-based job controller that spawns child processes and gracefully stops or restarts them based on signals.
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pid> <signal>\n", argv[0]);
        fprintf(stderr, "Signals: 1 = SIGUSR1, 2 = SIGUSR2, 9 = SIGTERM\n");
        exit(1);
    }

    pid_t pid = atoi(argv[1]);
    int sig_input = atoi(argv[2]);
    int sig;

    // Map 1,2,9 to actual signals
    if (sig_input == 1) sig = SIGUSR1;
    else if (sig_input == 2) sig = SIGUSR2;
    else if (sig_input == 9) sig = SIGTERM;
    else {
        fprintf(stderr, "Invalid signal number!\n");
        exit(1);
    }

    if (kill(pid, sig) == -1) {
        perror("kill");
        exit(1);
    }

    printf("Sent signal %d to process %d\n", sig, pid);
    return 0;
}