#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<signal.h>
#include<string.h>

void handle_signal(int signum){
    printf("Caught signal %d using signalaction()\n",signum);
}

int main(int argc,char *argv[]){
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sa.sa_flags=0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT,&sa,NULL);

    printf("Receiver pid: %d\n",getpid());
    printf("Waiting for signals: from sender ......\n");

    while (1)
    {
        pause();
    }
    return 0;
}