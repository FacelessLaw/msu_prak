#include "../list/list.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

/*void parse_list(char *** argv, plist p, int * fread, int * fwrite, int * bmode) {
    while (p) {
        if (strcmp(p->key, ">") || strcmp(p->key, ">>")) {
            *fwrite = open("")
        }
    }
}*/

plist parse_list(plist p, int * fr, int * fw, int * bmode) {
    *fr = *fw = -1;
    plist res = NULL;
    while (p) {
        if (p->type == BASH_MODE) {
            if (!strcmp(p->key, "&")) {
                *bmode = 1;
            } else if (!strcmp(p->key, ">>")) {
                printf("im here");
                if (*fw != -1) {
                    close(*fw);
                }
                *fw = open(p->next->key, O_WRONLY | O_APPEND | O_CREAT, S_IWRITE | S_IREAD);
            } else if (!strcmp(p->key, ">")) {
                if (*fw != -1) {
                    close(*fw);
                }
                *fw = open(p->next->key, O_WRONLY | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD);
            } else if (!strcmp(p->key, "<")) {
                if (*fr != -1) {
                    close(*fr);
                }
                *fr = open(p->next->key, O_RDONLY);
            }
            p = p->next;
        } else {
            res = add_word(res, p->key, p->type);
        }
        p = p->next;
    }
    return res;
}

void runproc(plist p) { //plist ./m >> 2 << 2 > 2 < 3 > 4 &
    int fr, fw, bm;
    
    plist res = parse_list(p, &fr, &fw, &bm);
        
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "can't fork\n");
        delete_list(res);
        if (fr != -1) {
            close(fr);
        }
        if (fw != -1) {
            close(fw);
        }
        return ;
    } else if (pid == 0) {
        char ** argv = ltoa(res);
        if (fr != -1) {
            dup2(fr, 0);
            close(fr);
        }
        if (fw != -1) {
            dup2(fw, 1);
            close(fw);
        }
        execvp(argv[0], argv);
        printf("%s:: Error\n", argv[0]);
    } else {
        wait(NULL);
        if (fr != -1) {
            close(fr);
        }
        if (fw != -1) {
            close(fw);
        }
        delete_list(res);
    }
}