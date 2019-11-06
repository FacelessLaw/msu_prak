#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "../list/list.h"

char * 
add_ch(char *s, int ch, int *len, int *sz) 
{
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

char * 
change_vals(char *s) 
{
    /* &PATH */
    /* TODO */
    return s;
}

void 
mode_quotes(
        int ch, 
        int * plastWasSpace, 
        char ** ps, 
        int * plen, 
        int *psz, 
        int *pmode, 
        int endCh) 
{
    if (ch == endCh) {
        /*if (*pmode == WAIT_PAIR) {
            *ps = to_norm(*ps);
        }*/
        *plastWasSpace = 0;
        *pmode = FREE;
    } else {
        if (ch == '\n') {
            printf("%s", CONT_LINE);
        }
        *ps = add_ch(*ps, ch, plen, psz);
    }
}

void 
mode_logic(
        int ch, 
        int * plastWasSpace, 
        char ** ps, 
        int * plen, 
        int *psz, 
        int *pmode, 
        int endCh,
        int *pBreakFlag,
        plist *presult) 
{
    *pBreakFlag = 0;
    if (ch == '\n') {
        *pmode = FREE;
        *pBreakFlag = 1;
        return;
    }
    if (ch == endCh) {
        *ps = add_ch(*ps, endCh, plen, psz);
        *presult = add_word(*presult, *ps);
        *plen = 0;
        *ps[*plen] = 0;
        *plastWasSpace = 1;
        *pmode = FREE;
    } else if (ch == '\'') {
        *presult = add_word(*presult, *ps);
        *plen = 0;
        *ps[*plen] = 0;
        *plastWasSpace = 1;
        *pmode = WAIT_ONES;
    } else if (ch == '\"') {
        *presult = add_word(*presult, *ps);
        *plen = 0;
        *ps[*plen] = 0;
        *plastWasSpace = 1;
        *pmode = WAIT_PAIR;
    } else if (ch == '\\') {
        *pmode = WAIT_ANY;
        *presult = add_word(*presult, *ps);
        *plen = 0;
        *ps[*plen] = 0;
        *plastWasSpace = 1;
        *pmode = FREE;
    } else if (isspace(ch)) {
        *presult = add_word(*presult, *ps);
        *plen = 0;
        *ps[*plen] = 0;
        *plastWasSpace = 1;
        *pmode = FREE;
    } else {
        *presult = add_word(*presult, *ps);
        *plen = 0;
        *ps[*plen] = 0;
        *ps = add_ch(*ps, ch, plen, psz);
        *plastWasSpace = 0;
        *pmode = FREE;
    }
}

plist 
parse_cmd() 
{
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
            } else if (ch == '\\') {
                mode = WAIT_ANY;
                lastWasSpace = 0;
            } else if (ch == '&') {
                mode = AMPER;
                if (!lastWasSpace) {
                    result = add_word(result, s);
                    len = 0;
                    s[len] = 0;
                }
                s = add_ch(s, '&', &len, &sz);
                lastWasSpace = 0;
            } else if (ch == '|') {
                mode = PIPE;
                if (!lastWasSpace) {
                    result = add_word(result, s);
                    len = 0;
                    s[len] = 0;
                }
                s = add_ch(s, '|', &len, &sz);
                lastWasSpace = 0;
            } else {
                s = add_ch(s, ch, &len, &sz);
                lastWasSpace = 0;
            }
        } else if (mode == WAIT_ONES) {
            mode_quotes(ch, &lastWasSpace, &s, &len, &sz, &mode, '\'');
        } else if (mode == WAIT_PAIR) {
            mode_quotes(ch, &lastWasSpace, &s, &len, &sz, &mode, '\"');
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
        } else if (mode == AMPER || mode == PIPE) {
            int endCh = (mode == AMPER ? '&' : '|'); 
            int breakFlag = 0;
            mode_logic(ch, &lastWasSpace, &s, &len, &sz, &mode, endCh, &breakFlag, &result);
            if (breakFlag) {
                break;
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
