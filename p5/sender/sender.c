#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>

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
    printf("Connecting...\n");
    int trying = 0;
    while ((msgid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)) == -1) {
        sleep(3);
        if (4 == ++trying) {
            printf("Can't connect\n");
            exit(1);
        }
    }
    printf("READY TO Write...\n");
    char msg[4][MSG_SIZE] = {
        "Hello\0", "How are you?\0", "Buy\0", "Q\0" 
    };
    Message.pid = getpid();
    for (int i = 0; i < 3; ++i) {
        strcpy(Message.s, msg[i]);
        Message.msgtype = 1;
        msgsnd(msgid, (struct msgbuf *)(&Message), sizeof(Message) - sizeof(long), 0);
        sleep(1);
    }
    return 0;
} 