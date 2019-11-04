#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/*char * read_word(FILE * fr) {
    int sz = 2;
    char *s = (char *) realloc(NULL, sizeof(*s) * sz);
    s[0] = 0;
    int ch = 0;
    int readingWasBegin = 0;
    while ((ch = getchar()) != EOF) {
        if (isspace(ch)) {
            if (readingWasBegin || ch == '\n') {
                break;
            } else {
                continue;
            }
        }
        if (ch == '\'' || ch == '\"') {
            while ((ch = getchar()) != EOF && )
        }

    }
    return s;
}*/

int check_ch(int ch, int endCh) {
    return (endCh && ch != endCh || !endCh && !isspace(ch));
}

char * add_ch(char *s, int ch, int *len, int *sz) {
    s[(*len)++] = ch;
    s[*len] = 0;
    if (*len + 1 == *sz) {
        *sz *= 2;
        s = (char *) realloc(s, sizeof(*s) * (*sz));
    }
    return s;
}

enum {
    WAIT_ONES = 0, /// '
    WAIT_PAIR = 1, /// ""
    WAIT_ANY = 2, /// \;
    FREE = 3, /// ...
};

const char * CONT_LINE = "~> ";
const char * ERROR_LINE[3] = {
    "\"\'\"", 
    "\"\"\"",
    "ANY ch. Last ch was \"\\\"",
};

char * read_word(int *contRead) {
    *contRead = 1;
    int sz = 2;
    char * s = (char *) realloc(NULL, sizeof(*s) * sz);
    s[0] = 0;
    int len = 0;
    int ch;
    while ((ch = getchar()) != EOF && isspace(ch)) {
        if (ch == '\n') {
            free(s);
            *contRead = 0;
            return NULL;
        }
    }
    ///While without \'
    int mode = FREE;
    int wasReading = 0;
    while (ch != EOF) {
        if (mode == FREE) {
            if (isspace(ch)) {
                if (ch == '\n') {
                    *contRead = 0;
                }
                break;
            }
            wasReading = 1;
            if (ch == '\'') {
                mode = WAIT_ONES;
            } else if (ch == '\"') {
                mode = WAIT_PAIR;
            }else if (ch == '\\') {
                mode = WAIT_ANY;
            } else {
                s = add_ch(s, ch, &len, &sz);
            }
        } else if (mode == WAIT_ONES) {
            if (ch == '\'') {
                mode = FREE;
            } else {
                if (ch == '\n') {
                    printf("%s", CONT_LINE);
                }
                s = add_ch(s, ch, &len, &sz);
            }
        } else if (mode == WAIT_PAIR) {
            /* 
                тут на самом деле работает еще вставка переменных
                например $PATH заменяется на значение переменной.
            */
            if (ch == '\"') {
                mode = FREE;
            } else {
                if (ch == '\n') {
                    printf("%s", CONT_LINE);
                }
                s = add_ch(s, ch, &len, &sz);
            }
        } else if (mode == WAIT_ANY) {
            if (ch != '\n') {
                s = add_ch(s, ch, &len, &sz);
            } else {
                printf("%s", CONT_LINE);
            }
            if (ch != '\\') {
                mode = FREE;
            }
        }
        ch = getchar();
    }
    if (mode != FREE) {
        fprintf(stderr, "Djarvis: Unexpected EOF during the search %s\n", ERROR_LINE[mode]);
        fprintf(stderr, "Djarvis: Syntax error \n");
        free(s);
        return NULL;
    }
    if (wasReading) {
        return s;
    }
    free(s);
    return NULL;
}
