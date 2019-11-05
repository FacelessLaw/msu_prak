#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "../list/list.h"

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
    WAIT_GRAVIS = 3, /// `;
    AMPER = 4, /// `;
    PIPE = 5, 
    FREE = 6, /// ...
};

const char * CONT_LINE = "~> ";
const char * ERROR_LINE[4] = {
    "\"\'\"", 
    "\"\"\"",
    "ANY ch. Last ch was \"\\\"",
    "\"`\"",
};

plist parse_cmd() {
    printf("Djarvis ~> ");
    
    plist result = NULL;
    int sz = 2;
    char * s = (char *) realloc(NULL, sizeof(*s) * sz);
    s[0] = 0;
    int len = 0;
    int ch;
    
    int mode = FREE;
    int wasReading = 0;
    int lastWasSpace = 1; 
    while ((ch = getchar()) != EOF) {
        if (mode == FREE) {
            if (isspace(ch)) {
                if (!lastWasSpace) {
                    //s = add_ch(s, ' ', &len, &sz);
                    result = add_word(result, s);
                    len = 0;
                    s[len] = 0;
                    lastWasSpace = 1;
                }
                if (ch == '\n') {
                    break;
                }
                
                continue;
            }
            wasReading = 1;
            if (ch == '\'') {                
                mode = WAIT_ONES;
                lastWasSpace = 0;
            } else if (ch == '\"') {
                mode = WAIT_PAIR;
                lastWasSpace = 0;
            }else if (ch == '\\') {
                mode = WAIT_ANY;
                lastWasSpace = 0;
            } else if (ch == '&') {
                mode = AMPER;
                if (!lastWasSpace) {
                    //s = add_ch(s, ' ', &len, &sz);
                    result = add_word(result, s);
                    len = 0;
                    s[len] = 0;
                }
                s = add_ch(s, '&', &len, &sz);
                lastWasSpace = 0;
            } else {
                s = add_ch(s, ch, &len, &sz);
                lastWasSpace = 0;
            }
        } else if (mode == WAIT_ONES) {
            if (ch == '\'') {
                lastWasSpace = 0;
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
                lastWasSpace = 0;
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
                lastWasSpace = 0;
                mode = FREE;
            }
        } else if (mode == AMPER) {
            if (ch == '\n') {
                mode = FREE;
                break;
            }
            if (ch == '&') {
                s = add_ch(s, '&', &len, &sz);
                result = add_word(result, s);
                len = 0;
                s[len] = 0;
                //s = add_ch(s, ' ', &len, &sz);
                lastWasSpace = 1;
                mode = FREE;
            } else if (ch == '\'') {
                result = add_word(result, s);
                len = 0;
                s[len] = 0;
                
                //s = add_ch(s, ' ', &len, &sz);
                lastWasSpace = 1;
                mode = WAIT_ONES;
            } else if (ch == '\"') {
                result = add_word(result, s);
                len = 0;
                s[len] = 0;
                
                //s = add_ch(s, ' ', &len, &sz);
                lastWasSpace = 1;
                mode = WAIT_PAIR;
            } else if (ch == '\\') {
                mode = WAIT_ANY;
                
                result = add_word(result, s);
                len = 0;
                s[len] = 0;
                
                //s = add_ch(s, ' ', &len, &sz);
                lastWasSpace = 1;
                mode = FREE;
            } else if (isspace(ch)) {
                result = add_word(result, s);
                len = 0;
                s[len] = 0;
                
                //s = add_ch(s, ' ', &len, &sz);
                lastWasSpace = 1;
                mode = FREE;
            } else {
                result = add_word(result, s);
                len = 0;
                s[len] = 0;
                
                //s = add_ch(s, ' ', &len, &sz);
                s = add_ch(s, ch, &len, &sz);
                lastWasSpace = 0;
                mode = FREE;
            }
        }
    }
    if (mode != FREE) {
        fprintf(stderr, "Djarvis: Unexpected EOF during the search %s\n", ERROR_LINE[mode]);
        fprintf(stderr, "Djarvis: Syntax error \n");
        free(s);
        return NULL;
    }
    if (wasReading) {
        if (ch == EOF) { //for files.
            result = add_word(result, s);
        }
        free(s);
        return result;
    }
    free(s);
    delete_list(result); // tut result na samom dele doljen byt ravnym NULL
    return NULL;
}
