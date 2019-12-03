#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>

enum {
    MAGIC_NUM = 29,
    MSG_SIZE = 20
};

struct {
    long msgtype;
    int pid;
    char s[MSG_SIZE];
} Message;

int main(void) {
    //key_t key = ftok("/etc/password", MAGIC_NUM);
    key_t key = MAGIC_NUM;
    int msgid;
    if ((msgid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)) == -1) {
        fprintf(stderr, "MSGGET ERROR\n");
        exit(1);
    }
    printf("READY TO READ...\n");
    for (;;) {
        msgrcv(msgid, (struct msgbuf *)(&Message), sizeof(Message) - sizeof(long), 1, 0);
        printf("[%d] said --> %s\n", Message.pid, Message.s);
        if (Message.s[0] == 'q') {
            break;
        }
    }
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
} 