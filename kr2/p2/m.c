#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

enum {
    DF = 0,
    SORT = 1,
    GREP = 2,
    C_CMDS = 3
};

int main() {
    int fread;
    int fd[2];
    fread = dup(0);
    for (int i = 0; i < C_CMDS; ++i) {
        if (i < 2) {
            pipe(fd);
        } else {
            fd[1] = open("task2.txt", O_WRONLY | O_CREAT, S_IWRITE | S_IREAD);
        }
        int pid = fork();
        if (pid < 0) {
            exit(1);
        } else if (pid == 0) {
            dup2(fread, 0);
            dup2(fd[1], 1);
            switch(i) {
                case DF: execlp("df", "df", "-h", NULL); break;
                case SORT: execlp("sort", "sort", NULL); break;
                case GREP: execlp("grep", "grep", "--", "o", NULL ); break;
            }
            exit(1);
        }
        close(fread);
        close(fd[1]);
        fread = fd[0];
    }
    close(fread);
    for (int i = 0; i < C_CMDS; ++i) {
        wait(NULL);
    }
    return 0;
}