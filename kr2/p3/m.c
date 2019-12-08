#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>

int READING_WAS = 0;
int fd[2];

void sigu(int signal) {
    READING_WAS = 1;
}

void sigk(int signal) {
    close(fd[0]);
    close(fd[1]);
    exit(0);
}

int main() {
    int N;
    scanf("%d", &N);
    signal(SIGUSR1, sigu);
    signal(SIGKILL, sigk);
    pipe(fd);
    
    int pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (pid == 0) {
        int val;
        pid = getpid();
        int ppid = getppid();
        while (read(fd[0], &val, sizeof(val))) {
            fprintf(stderr, "SON: [%d] --> %d \n", pid, val);
            sleep(1);
            kill(ppid, SIGUSR1);
        }
        return 0;
    } else {
        int mypid = getpid();
        for (int i = 1; i <= N; ++i) {
            write(fd[1], &i, sizeof(i));
            fprintf(stderr, "PAR: [%d] --> %d\n", mypid, i);
            while (!READING_WAS) { }
            READING_WAS = 0;
        }
        kill(pid, SIGKILL);
        wait(NULL);
    }
}