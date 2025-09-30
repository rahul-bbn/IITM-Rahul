#define _POSIX_C_SOURCE 2008009L
#include<stdio.h>
#include<stdlib.h>
#include<stdatomic.h>
#include<string.h>
#include<errno.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<time.h>
#include<unistd.h>

#define SHM_TEXT_CAP 256
#define FTOK_PATH "./shm_demo_keyfile"
#define FTOK_PROJID 0x42

typedef struct{
    _Atomic int ready;
    size_t length;
    char text[SHM_TEXT_CAP];
}ShmPayload;


static void die(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}



/*Q1. What does shmget() function return on success?
Q2. What does shmat() function return on success?
Q3. The value of segment size in shmget() is a round-up value to a multiple of ____.
Q4. Can a process create two shared segment? Will there returned identifiers be same or different?
Q5. Which function can be used to detach the shared segment from the address space of the process?

Q1. Write a program to create a shared memory segment of 2048 bytes and write some content into it. Then create a child process which then reads the content written by the parent process in the shared memory segment
*/