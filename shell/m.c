#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include "tools/list/list.h"
#include "tools/cmdline/cmdline.h"
#include "tools/runproc/runproc.h"

const char * PARSE = "-p\0";
const char * PARSE_INF = "-pi\0";
const char * RUN = "-r\0";
const char * HELP = "-h\0";

plist openProc = NULL;
plist closeProc = NULL;

void listener(int sig) {
    int status;
    int pid = wait(&status);
    plist node = NULL;
    openProc = del_pid(openProc, pid, &node);
    if (node) {
        closeProc = add_word(closeProc, node->key, node->type);
        free(node->key);
        free(node);
    }
    return ;
}

int 
main(int argc, char *argv[]) {
    int wasEOF = 0;
    if (argc >= 2) {
        if (!strcmp(argv[1], PARSE)) {
            printf("\n");
            plist cmd = parse_cmd(&wasEOF);
            printf("\n");
            print_list(cmd);
            delete_list(cmd);
            printf("\n");
            return 0;
        } else if (!strcmp(argv[1], RUN)) {
            plist cmd = parse_cmd(&wasEOF);
            runproc(cmd);
            delete_list(cmd);
            return 0;
        } else if (!strcmp(argv[1], HELP)) {
            printf("USAGE: ./m [-r, -p, -h]\n\n");
            printf("\t -r \t run command\n\n");
            printf("\t -p \t print parse command\n\n");
            printf("\t -h \t print help\n\n");
            return 0;
        } else if (!strcmp(argv[1], PARSE_INF)) {
            while (!wasEOF) {
                printf("\n");
                plist cmd = parse_cmd(&wasEOF);
                printf("\n");
                print_list(cmd);
                delete_list(cmd);
            }
            printf("\n");
            return 0;
        }
    }
    //signal(SIGINT, listener);
    signal(SIGCHLD, listener);
    wasEOF = 0;
    while (!wasEOF) {
        plist cmd = parse_cmd(&wasEOF);
        runproc(cmd);
        delete_list(cmd);
        print_loop(closeProc);
        delete_list(closeProc);
        closeProc = NULL;
    }
    printf("\n");
    return 0;
}