#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "tools/list/list.h"
#include "tools/tree/tree.h"
#include "tools/cmdline/cmdline.h"
#include "tools/runproc/runproc.h"

const char * PARSE = "-p\0";
const char * PARSE_INF = "-pi\0";
const char * RUN = "-r\0";
const char * HELP = "-h\0";

plist openProc = NULL;
plist closeProc = NULL;

void mv_proc(int pid, int status) {
    plist node = NULL;
    openProc = del_pid(openProc, pid, &node);
    if (node) {
        node->next = closeProc;
        node->status = status;
        closeProc = node;
    }
}

void 
sigchld_listener(int sig) {
    int status;
    int pid = waitpid(-1, &status, WNOHANG);
    if (!pid) {
        return ;
    }
    mv_proc(pid, status);
    return ;
}

void delete_all() {
    plist tmp = openProc;
    while (tmp) {
        int status;
        int pid = tmp->type;
        kill(pid, 9);
        waitpid(pid, &status, 0);
        print_killing(tmp);
        tmp = tmp->next;
    }
    delete_list(openProc);
    openProc = NULL;
}

void sigint_listener(int sig) {
    //delete_all();
}

__sighandler_t sigh;

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
            sigh = signal(SIGINT, sigint_listener);
            signal(SIGCHLD, sigchld_listener);
            wasEOF = 0;
            while (!wasEOF) {
                plist cmd = parse_cmd(&wasEOF);
                //runproc(cmd);
                runpipe(cmd);
                delete_list(cmd);
                if (closeProc) {
                    print_loop(closeProc);
                    delete_list(closeProc);
                    closeProc = NULL;
                }
            }
            printf("\n");
            delete_all();
            print_loop(openProc);
            print_loop(closeProc);
            delete_list(closeProc);                
            return 0;
        } else if (!strcmp(argv[1], HELP)) {
            printf("USAGE: ./m [-r, -p, -h]\n\n");
            printf("\t -r \t run command\n\n");
            printf("\t -p \t print parse command\n\n");
            printf("\t -h \t print help\n\n");
            return 0;
        } else if (!strcmp(argv[1], PARSE_INF)) {
            while (!wasEOF) {
                plist cmd = parse_cmd(&wasEOF);
                ptree p = make_tree(cmd);
                print_tree(p);
                printf("\n");
                delete_tree(p);            
            }
            printf("\n");
            return 0;
        }
    }
    /*sigh = signal(SIGINT, sigint_listener);
    signal(SIGCHLD, sigchld_listener);
    wasEOF = 0;
    
    while (!wasEOF) {
        plist cmd = parse_cmd(&wasEOF);
        ptree p = make_tree(cmd);
        //printf("*******************************************\n");
        runtree(p, -1, -1, 0);
        delete_tree(p);    
    }
    printf("\n");
    */

    sigh = signal(SIGINT, sigint_listener);
    signal(SIGCHLD, sigchld_listener);
    wasEOF = 0;
    while (!wasEOF) {
        plist cmd = parse_cmd(&wasEOF);
        ptree p = make_tree(cmd);
        runtree(p, -1, -1, 0);
        delete_tree(p);
        
        if (closeProc) {
            print_loop(closeProc);
            delete_list(closeProc);
            closeProc = NULL;
        }
    }
    printf("\n");
    delete_all();
    print_loop(openProc);
    print_loop(closeProc);
    delete_list(closeProc);                
    return 0;
}
