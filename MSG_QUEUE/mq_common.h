#ifdef MQ_COMMON_H
#define MQ_COMMON_H
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include<errno.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>

#define MQ_PATH "./tmp"
#define MQ_PROJ_ID 'H'

//code for operation
eum{
    OP_PING=1,
    OP_ADD=2
};

//structure for request message
typedef struct{
    long type;
    pid_t pid;
    int op;
    int a;
    int b;
    char text[64];
}req_msg_t;

#endif

