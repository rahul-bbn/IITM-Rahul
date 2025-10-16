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
    int sig = atoi(argv[2]);

    if (kill(pid, sig) == -1) {
        perror("kill");
        exit(1);
    }

    printf("Sent signal %d to process %d\n", sig, pid);
    return 0;
}