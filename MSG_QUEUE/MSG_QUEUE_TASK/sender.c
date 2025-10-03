#include "msg_common.h"

int main() {
    key_t key;
    int msgid;
    struct msg_buffer msg;

    key = ftok("chatfile", 65);

    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid < 0) {
        perror("msgget");
        exit(1);
    }

    printf("User1 ready. Type messages (type 'exit' to quit):\n");

    if (fork() == 0) {

        while (1) {
            msgrcv(msgid, &msg, sizeof(msg.mtext), 2, 0); 
            if (strncmp(msg.mtext, "exit", 4) == 0) {
                printf("User2 exited. Closing chat.\n");
                break;
            }
            printf("User2: %s\n", msg.mtext);
        }
        exit(0);
    } else {
        
        while (1) {
            fgets(msg.mtext, MAX_SIZE, stdin);
            msg.mtext[strcspn(msg.mtext, "\n")] = '\0'; 
            msg.mtype = 1; 
            msgsnd(msgid, &msg, sizeof(msg.mtext), 0);

            if (strncmp(msg.mtext, "exit", 4) == 0) {
                break;
            }
        }
    }

    return 0;
}