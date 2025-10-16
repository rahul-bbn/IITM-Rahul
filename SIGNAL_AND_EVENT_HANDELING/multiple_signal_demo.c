#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int signum) {
    if (signum == SIGINT)
        printf("Caught SIGINT (Ctrl + C)\n");
    else if (signum == SIGTERM)
        printf("Caught SIGTERM (Termination request)\n");
    else if (signum == SIGUSR1)
        printf("Caught SIGUSR1 (Custom user-defined signal)\n");
    else if (signum == SIGUSR2)
        printf("Caught SIGUSR2 (Custom user-defined signal)\n");
}

int main() {
    struct sigaction sa;

    sa.sa_handler = handle_signal;     // function to handle signals
    sa.sa_flags = 0;                   // default behavior
    sigemptyset(&sa.sa_mask);          // no additional signals blocked

    // Register handlers for multiple signals
    sigaction(SIGINT, &sa, NULL);      // Ctrl + C
    sigaction(SIGTERM, &sa, NULL);     // kill -15 <pid>
    sigaction(SIGUSR1, &sa, NULL);     // kill -USR1 <pid>
    sigaction(SIGUSR2, &sa, NULL);     // kill -USR2 <pid>

    printf("PID: %d\n", getpid());
    printf("Try sending signals using:\n");
    printf("  kill -SIGINT  %d   (Ctrl + C equivalent)\n", getpid());
    printf("  kill -SIGTERM %d   (termination request)\n", getpid());
    printf("  kill -SIGUSR1 %d   (user-defined signal 1)\n", getpid());
    printf("  kill -SIGUSR2 %d   (user-defined signal 2)\n", getpid());
    printf("----------------------------------------------\n");

    // Infinite loop: wait for signals
    while (1) {
        printf("Running...\n");
        sleep(2);
    }

    return 0;
}
