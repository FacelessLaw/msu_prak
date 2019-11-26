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
#include "../tree/tree.h"
#include "../runproc/runproc.h"

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
    *bmode = (p && p->type == AMP);
    return res;
}

int pids[200];
int cntProc;

int runproc(plist p, int fr, int fw, int pipe) { //plist ./m >> 2 << 2 > 2 < 3 > 4 &
    if (!p) {
        return OK_CODE;
    }
    int bm = 0;
    plist res = parse_list(p, &fr, &fw, &bm);
    char ** argv = ltoa(res);
    if (!strcmp(argv[0], "cd")) {
        if (argv[1]) {
            if (argv[2]) {
                fprintf(stderr, "Djarvis Error:: Too many arguments for cd\n");
                return ERROR_CODE;
            }
            chdir(argv[1]);
        } else {
            chdir(getenv("HOME"));
        }
        delete_list(res);
        free(argv);
        return OK_CODE;
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "can't fork\n");
        delete_list(res);
        free(argv);
        if (fr != -1) {
            close(fr);
        }
        if (fw != -1) {
            close(fw);
        }
        return ERROR_CODE;
    } else if (pid == 0) {
        if (fr != -1) {
            dup2(fr, 0);
            close(fr);
        }
        if (fw != -1) {
            dup2(fw, 1);
            close(fw);
        }
        if (bm) {
            if (fr == -1) {
                fr = open("/dev/null", O_RDONLY);
                dup2(fr, 0);
                close(fr);
            }
        }
        execvp(argv[0], argv);
        fprintf(stderr, "%s:: Error\n", argv[0]);
        return ERROR_CODE;
    } else {
        if (fr != -1) {
            close(fr);
        }
        if (fw != -1) {
            close(fw);
        }
        int status = 0;
        int resStatus = 0;
        if (bm) {
            openProc = add_word(openProc, argv[0], pid);
            resStatus = OK_CODE;
        } else if (pipe) {
            pids[cntProc++] = pid;
            resStatus = OK_CODE;
        } else {
            waitpid(pid, &status, 0);
            resStatus = (WIFEXITED(status) && !WEXITSTATUS(status));
        }
        free(argv);
        delete_list(res);
        
        return resStatus;
    }
}

int runtree(ptree p, int fr, int fw, int pip) {
    if (!p) {
        return OK_CODE;
    }
    if (!p->lt && !p->rt) {
        return runproc(p->key, fr, fw, pip) == OK_CODE;
    } else if (p->key->type == LOGIC_AND) {
        if (runtree(p->lt, fr, fw, pip) == OK_CODE) {
            if (runtree(p->rt, fr, fw, pip) == OK_CODE) {
                return OK_CODE;
            }
        }
        return  ERROR_CODE;
    } else if (p->key->type == LOGIC_OR) {
		if (runtree(p->lt, fr, fw, pip) == OK_CODE) {
            return OK_CODE;
        }
        if (runtree(p->rt, fr, fw, pip) == OK_CODE) {
            return OK_CODE;
        }
        return  ERROR_CODE;
    } else if (p->key->type == PIPE) {
        int fd[2];
        pipe(fd);
        if (!runtree(p->lt, fr, fd[1], 1)) {
            printf("Strange Error with A in A | B...\n");
            return ERROR_CODE;
        } else if (!runtree(p->rt, fd[0], fw, pip)) {
            printf("Strange Error with A in A | B...\n");
            return ERROR_CODE;
        }
        if (!pip) {
            int resStatus = OK_CODE;
            int i;
            for (i = 0; i < cntProc; ++i) {
                int status = 0;
                waitpid(pids[i], &status, 0);
                if (!WIFEXITED(status) || WEXITSTATUS(status)) {
                    resStatus = ERROR_CODE;
                }
            }
            cntProc = 0;
            return resStatus;
        }
        return OK_CODE;
    }
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
    int i;
    for (i = 0; i < cntProc; ++i) {
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
        for (i = 0; i < cntProc; ++i) {
            waitpid(pids[i], NULL, 0);
        }
    } else {
        printf("[%d] %d\n", 28, pid);
        for (i = 0; i < cntProc; ++i) {
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
