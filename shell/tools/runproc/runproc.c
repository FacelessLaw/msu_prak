#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

#include "../list/list.h"

/*void parse_list(char *** argv, plist p, int * fread, int * fwrite, int * bmode) {
    while (p) {
        if (strcmp(p->key, ">") || strcmp(p->key, ">>")) {
            *fwrite = open("")
        }
    }
}*/

extern plist openProc; 

plist parse_list(plist p, int * fr, int * fw, int *bmode) {
    plist res = NULL;
    while (p && (p->type == WORD || p->type == STREAM)) {
        if (p->type == STREAM) {
            if (!strcmp(p->key, ">>")) {
                close(*fw);
                *fw = open(p->next->key, O_WRONLY | O_APPEND | O_CREAT, S_IWRITE | S_IREAD);
                p = p->next;
            } else if (!strcmp(p->key, ">")) {
                close(*fw);
                *fw = open(p->next->key, O_WRONLY | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD);
                p = p->next;
            } else if (!strcmp(p->key, "<")) {
                close(*fr);
                *fr = open(p->next->key, O_RDONLY);
                p = p->next;
            }
        } else {
            res = add_word(res, p->key, p->type);
        }
        p = p->next;
    }
    return res;
}

void runpipe(plist  p) { // A | B | C
    if (!p) {
        return; 
    }
    int cntProc = 1;
    plist end = p; 
    while (end && end->type != AMP) {
        if (end->type == PIPE) {
            ++cntProc;
        }
        end = end->next;
    }
    int bm = end && end->type == AMP;
    int * pids = (int*) malloc(cntProc * sizeof(*pids));
    char ** commands = (char**) malloc(
        cntProc * sizeof(*commands)
    );
    int pid;
    int fd[2]; //curr proc write to fd1[1]
    int fread; //curr proc read from fd0[0]
    if (bm) {
        fread = open("/dev/null", O_RDONLY);
    } else {
        fread = dup(0);
    }
    for (int i = 0; i < cntProc; ++i) {
        plist tmp = p;
        while (tmp != end && tmp->type != PIPE) {
            tmp = tmp -> next;
        }
        if (i + 1 < cntProc) {
            pipe(fd);
        } else {
           fd[1] = dup(1);
        }
        plist res = parse_list(p, &fread, &fd[1], &bm);
        char ** argv = ltoa(res);
        if (!strcmp(argv[0], "cd")) {
            if (argv[1]) {
                if (argv[2]) {
                    fprintf(stderr, "Djarvis Error:: Too many arguments for cd\n");
                    return ;
                }
                chdir(argv[1]);
            } else {
                chdir(getenv("HOME"));
            }
            delete_list(res);
            free(argv);
            close(fread);
            close(fd[1]);
            fread = fd[0];
            p = (tmp ? tmp->next : tmp);
            continue; 
        }
        pid = fork();
        if (pid < 0) {
            exit(1);
        } else if (pid == 0) {
            dup2(fread, 0);
            dup2(fd[1], 1);
            close(fread);
            close(fd[0]);
            close(fd[1]);
            execvp(*argv, argv);
            fprintf(stderr, "%s:: Error\n", argv[0]);
            exit(1);
        }
        
        pids[i] = pid;
        commands[i] = p->key;
        
        close(fread);
        close(fd[1]);
    
        fread = fd[0];
        
        delete_list(res);
        p = (tmp ? tmp->next : tmp);
        free(argv);
    }
    close(fread);
    if (!bm) {
        for (int i = 0; i < cntProc; ++i) {
            waitpid(pids[i], NULL, 0);
        }
    } else {
        printf("[%d] %d\n", 28, pid);
        for (int i = 0; i < cntProc; ++i) {
            openProc = add_word(
                    openProc, 
                    commands[i], 
                    pids[i]
            );
        }
    }
    free(pids);
    free(commands);
}
