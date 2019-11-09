#include "../list/list.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h>

/*void parse_list(char *** argv, plist p, int * fread, int * fwrite, int * bmode) {
    while (p) {
        if (strcmp(p->key, ">") || strcmp(p->key, ">>")) {
            *fwrite = open("")
        }
    }
}*/

void runproc(plist p) { //plist
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "can't fork\n");
        return ;
    } else if (pid == 0) {
        char ** argv = ltoa(p);
        execvp(argv[0], argv);
        printf("%s:: Error\n", argv[0]);
    } else {
        wait(NULL);
    }
}