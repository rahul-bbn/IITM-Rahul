#ifndef MSG_COMMON_H
#define MSG_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MAX_SIZE 100

struct msg_buffer {
    long mtype;
    char mtext[MAX_SIZE];
};

#endif