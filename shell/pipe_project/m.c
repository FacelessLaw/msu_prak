#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
    int fread; //curr proc read from fd0[0]
    int fd[2]; //curr proc write to fd1[1]
    fread = dup(0);
    for (int i = 1; i < argc; ++i) {
        if (i + 1 < argc) {
            pipe(fd);
        } else {
           fd[1] = dup(1); 
        }
        int pid = fork();
        if (pid < 0) {
            exit(1);
        } else if (pid == 0) {
            dup2(fread, 0);
            dup2(fd[1], 1);
            close(fread);
            close(fd[0]);
            close(fd[1]);
            execlp(argv[i], argv[i], NULL);
            exit(1);
        }
        close(fread);
        close(fd[1]);
        fread = fd[0];
    }
    close(fread); 
    while (wait(NULL) != -1);
}
