#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools/list/list.h"
#include "tools/cmdline/cmdline.h"


enum {
    ARG_MAX = 2097152,
    ARG_CNT = 2,
    A_FILE_R = 1
};

char CMD_SEP[10] = " \n";

char *
readStr(FILE * fr) {
    int sz = 2;
    char *s = (char *) realloc(NULL, sizeof(*s) * sz);
    s[0] = 0;
    while (fgets(s + strlen(s), sz - strlen(s), fr)) {
        sz *= 2;
        s = realloc(s, sz);
        if (s[strlen(s) - 1] == '\n') {
            break;
        }
    }
    int len = strlen(s);
    if (len == 0) {
        free(s);
        return NULL;
    }
    /*if (s[len - 1] == '\n') {
        s[len - 1] = 0;
    }*/
    return s;
}

char ** 
parseWords(char * cmd) {
    if (!cmd) { 
        return NULL; 
    }
    int sz = 2;
    int len = 0;
    char ** res = (char **) realloc(NULL, sizeof(*res) * sz);
    res[len] = strtok(cmd, CMD_SEP);
    if (!res[len]) {
        res[len] = cmd;
        res[len + 1] = NULL;
        return res;
    }
    ++len;
    while ((res[len++] = strtok(NULL, CMD_SEP))) {
        if (len == sz) {
            sz *= 2;
            res = (char **) realloc(res, sizeof(*res) * sz);
        }
    }
    return res;
}

void TEST_LIST() {
    plist root = NULL;
    root = add_word(root, "garik");
    root = add_word(root, "sharik");
    print_list(root);
    delete_list(root);
}

int 
main(int argc, char *argv[]) {
    /*if (argc != ARG_CNT) {
        fprintf(stderr, "I need %d args\n", ARG_CNT);
        return 1; 
    }
    FILE * fr;
    if (!(fr = fopen(argv[A_FILE_R], "r"))) {
        fprintf(stderr, "I can't open file %s\n", argv[A_FILE_R]);
        return 1;
    }
    char * str = readStr(fr);
    fclose(fr);
    char ** cmdWords = parseWords(str);
    char ** word = cmdWords;
    while (word && *word) {
        printf("%s\n", *(word++));
    }
    free(str);
    free(cmdWords);
    */
    
    
    /*
    TEST_LIST();
    */
    printf("Djarvis ~> ");
    plist root = NULL;
    char * word = NULL;
    int contRead = 1;
    while (word = read_word(&contRead)) {
        root = add_word(root, word);
        free(word);
        if (!contRead) {
            break;
        }
    }
    print_list(root);
    delete_list(root);
    return 0;
}