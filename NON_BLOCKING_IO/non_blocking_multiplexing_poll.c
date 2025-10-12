#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<poll.h>

int main(){
    int fd;
    char buff[100];
    fd=open("test.txt",O_RDONLY);

    if (fd<0)
    {
        perror("open");
        exit(1);
    }

    // make stdin non block
    int flag=fcntl(STDIN_FILENO,F_GETFL,0);
    fcntl(STDIN_FILENO,F_SETFL,flag | O_NONBLOCK)

    //fd non block
    flag=fcntl(fd,F_GETFL,0);
    fcntl(fd,F_SETFL,falg | O_NONBLOCK);

    struct pollfd fds[2];

    fds[0].fd=STDIN_FILENO;
    fds[0].events=POLLIN;

    fds[1].fd=fd;
    fds[1].events=POLLIN;


    
}