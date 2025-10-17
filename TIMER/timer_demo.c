#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void timer_handler(int signum) {
    static int count = 0;
    printf("Timer fired %d times\n", ++count);
}

int main() {
    struct sigaction sa;
    struct itimerval timer;

    sa.sa_handler = &timer_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);

    timer.it_value.tv_sec = 2;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 3;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, NULL);

    printf("Timer started. PID: %d\n", getpid());

    while (1) {
        sleep(1);
    }

    return 0;
}