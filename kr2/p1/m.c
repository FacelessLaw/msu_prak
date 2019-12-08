#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>

const char cmd[10] = "echo\0";

enum {
    MAX_NUM_SIZE = 100
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }
    int n = atoi(argv[1]);
    if (!n) {
        return 1;
    }
    char ** av = (char **) malloc(sizeof (*av) *(n + 2));

    int cBytes = sizeof(*av[0]) * MAX_NUM_SIZE;
    av[0] = (char *) malloc(cBytes);
    memset(av[0], 0, cBytes);
    strcpy(av[0], cmd);
    for (int i = 1; i <= n; ++i) {
        av[i] = (char *) malloc(cBytes); 
        memset(av[i], 0, cBytes);
        sprintf(av[i], "%d", i);
    }
    av[n + 1] = NULL;
    execvp(av[0], av);
    for (int i = 0; i <= n; ++i) {
        free(av[i]);
    }
    free(av);
    return 0;
}