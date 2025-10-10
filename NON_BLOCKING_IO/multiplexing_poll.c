#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
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
    struct pollfd fds[2];

    fds[0].fd=STDIN_FILENO;
    fds[0].events=POLLIN;

    fds[1].fd=fd;
    fds[1].events=POLLIN;

    printf("Monitoring stdin and file for input using poll()\n");

    while (1)
    {
        int ret=poll(fds,2,-1);

        if (ret<0)
        {
            perror("poll");
            exit(1);
        }
        if (fds[0].revents & POLLIN)
        {
            int n=read(STDIN_FILENO,buff,sizeof(buff)-1)
            if(n>0){
                buff[n]='0';
                printf("\nyou typed:%s\n",buff);
            }
        }
        if (fds[1].revents & POLLIN)
        {
            int n=read(fd,buff,sizeof(buff)-1);
            if(n>0){
                buff[n]='\0';
                printf("file content:%s\n",buff);
            }
        }  
    }
    close(fd);
    return 0;
    
}