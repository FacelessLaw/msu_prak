#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    A_CNT = 4,
    A_FILE_R = 1,
    A_FILE_W = 2,
    A_STR_DEL = 3,
};


char *readStr(FILE *fr) {
    int sz = 2;
    char *s = (char *) realloc(NULL, sizeof(*s) * sz);
    s[0] = 0;
    while (fgets(s + strlen(s), sz - strlen(s), fr)) {
        sz *= 2;
        s = realloc(s, sz);
    }
    if (strlen(s) == 0) {
        free(s);
        s = NULL;
    }
    return s;
}

void printStr(char *s, char *from, FILE *fw) {
    char *ps = s;
    int dps = strlen(from);
    while (ps && *ps) {
        char *fnd = strstr(ps, from);
        if(fnd) {
            char tmp = *fnd;
            *fnd = 0;
            fputs(ps, fw);
            *fnd = tmp;
            ps = fnd + dps;
        } else {
            fputs(ps, fw);
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != A_CNT) {
        fprintf(stderr, "\n I need %d args \n", A_CNT);
        exit(1);
    }
    FILE *fr = fopen(argv[A_FILE_R], "r");
    if (!fr) {
        fprintf(stderr, "\n I can't open file: %s  \n", argv[A_FILE_R]);
        exit(1);
    }
    FILE *fw = fopen(argv[A_FILE_W], "w");
    if (!fw) {
        fprintf(stderr, "\n I can't open file: %s  \n", argv[A_FILE_R]);
        exit(1);
    }
    char *s = NULL;
    while ((s = readStr(fr))) {
        printStr(s, argv[A_STR_DEL], fw);
        free(s);
    }
    fclose(fr);
    fclose(fw);
    return 0;
}