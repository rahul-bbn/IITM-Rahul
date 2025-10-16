#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void handle_signal(int signum){
    printf("Caught signal %d using sigaction()\n", signum);
}

int main(){
    struct sigaction sa;

    sa.sa_handler = handle_signal;
    sa.sa_flags=0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT,&sa,NULL);

    printf("PID: %d - waiting for SIGINT (press ctrl + c)......\n",getpid());

    while (1) {
        printf("Running...\n");
        sleep(2);
    }
    return 0;
}