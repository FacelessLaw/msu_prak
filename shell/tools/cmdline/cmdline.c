#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
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
    PAIR_CH = 4, // (&, |, >);
    FREE = 5, /// ...
};

const char * CONT_LINE = "~> ";
const char * ERROR_LINE[4] = {
    "\"\'\"", 
    "\"\"\"",
    "ANY ch. Last ch was \"\\\"",
    "\"`\"",
};
const char PAIR_CHARS[5] = "|&>\0"; 
const char ONES_CHARS[6] = "<;,()\0"; 

char * 
change_vals(char *s) 
{
    /* &PATH */
    /* TODO */
    return s;
}

void 
mode_any(
        int ch, 
        int * plastWasSpace, 
        char ** ps, 
        int * plen, 
        int *psz, 
        int *pmode)
{
    if (ch != '\n') {
        *ps = add_ch(*ps, ch, plen, psz);
    } else {
        printf("%s", CONT_LINE);
    }
    if (ch != '\\') {
        *plastWasSpace = 0;
        *pmode = FREE;
    }
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

void add_word_to_res(plist *presult, char **ps, int *plen, int type) {
    *presult = add_word(*presult, *ps, type);
    *plen = 0;
    *ps[*plen] = 0;
}

void 
mode_pair(
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
    int cmd_mode = STREAM; // & > < >>
    if (ch == endCh) {
        *ps = add_ch(*ps, endCh, plen, psz);
        if (ch != '>') {
            cmd_mode = BASH;
        }
    } else if (endCh == '|') {
        cmd_mode = PIPE;
    }
    add_word_to_res(presult, ps, plen, cmd_mode);
    if (ch == '\n') {
        *pBreakFlag = 1;
        *pmode = FREE;
        return;
    }
    if (ch == endCh) {
        *pmode = FREE;
        *pBreakFlag = 0;
        *plastWasSpace = 1;
    } else if (strchr(PAIR_CHARS, ch)) {
        *ps = add_ch(*ps, ch, plen, psz);
        
        *pmode = PAIR_CH;
        *pBreakFlag = 0;
        *plastWasSpace = 0;
    } else if (ch == '\'') {
        *pmode = WAIT_ONES;
        *pBreakFlag = 0;
        *plastWasSpace = 1;
    } else if (ch == '\"') {
        *pmode = WAIT_PAIR;
        *pBreakFlag = 0;
        *plastWasSpace = 1;
    } else if (ch == '\\') {
        *pmode = FREE;
        *pBreakFlag = 0;
        *plastWasSpace = 1;
    } else if (isspace(ch)) {
        *pmode = FREE;
        *pBreakFlag = 0;
        *plastWasSpace = 1;
    } else if (strchr(ONES_CHARS, ch)) {
        *ps = add_ch(*ps, ch, plen, psz);
        add_word_to_res(presult, ps, plen, BASH);

        *pmode = FREE;
        *pBreakFlag = 0;
        *plastWasSpace = 1;
    } else {
        *ps = add_ch(*ps, ch, plen, psz);
        
        *pmode = FREE;
        *pBreakFlag = 0;
        *plastWasSpace = 0;
    }
}

void 
mode_free(
        int ch, 
        int * plastWasSpace, 
        char ** ps, 
        int * plen, 
        int *psz, 
        int *pmode, 
        int *pBreakFlag,
        int *pwasReading,
        plist *presult)
{
    if (isspace(ch)) {
        if (!*plastWasSpace) {
            add_word_to_res(presult, ps, plen, WORD);
            *plastWasSpace = 1;
        }
        if (ch == '\n') {
            *pBreakFlag = 1;
            return ;
        }
        return;
    }
    *pwasReading = 1;
    if (ch == '\'') {                
        *pmode = WAIT_ONES;
        *plastWasSpace = 0;
    } else if (ch == '\"') {
        *pmode = WAIT_PAIR;
        *plastWasSpace = 0;
    } else if (ch == '\\') {
        *pmode = WAIT_ANY;
        *plastWasSpace = 0;
    } else if (strchr(PAIR_CHARS, ch)) {
        *pmode = PAIR_CH;
        if (!*plastWasSpace) {
            add_word_to_res(presult, ps, plen, WORD);
        }
        *ps = add_ch(*ps, ch, plen, psz);
        *plastWasSpace = 0;
    } else if (strchr(ONES_CHARS, ch)) {
        *pmode = FREE;
        if (!*plastWasSpace) {
            add_word_to_res(presult, ps, plen, WORD);
        }
        *ps = add_ch(*ps, ch, plen, psz);
        int cmd_mode = BASH;
        if (ch == '<') {
            cmd_mode = STREAM;
        }
        add_word_to_res(presult, ps, plen, cmd_mode);
        *plastWasSpace = 1;
    } else {
        *ps = add_ch(*ps, ch, plen, psz);
        *plastWasSpace = 0;
    }
}


plist 
parse_cmd(int * wasEOF) 
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
            int breakFlag = 0;
            mode_free(
                ch, 
                &lastWasSpace, 
                &s, &len, 
                &sz, 
                &mode,
                &breakFlag, 
                &wasReading, 
                &result
            );
            if (breakFlag) {
                break;
            }
        } else if (mode == WAIT_ONES) {
            mode_quotes(
                ch, 
                &lastWasSpace, 
                &s, 
                &len, 
                &sz, 
                &mode, 
                '\''
            );
        } else if (mode == WAIT_PAIR) {
            mode_quotes(
                ch, 
                &lastWasSpace, 
                &s, 
                &len, 
                &sz, 
                &mode, 
                '\"'
            );
        } else if (mode == WAIT_ANY) {
            mode_any(ch, &lastWasSpace, &s, &len, &sz, &mode);
        } else if (mode == PAIR_CH) {
            int endCh = s[len - 1]; 
            int breakFlag = 0;
            mode_pair(
                ch, 
                &lastWasSpace, 
                &s, 
                &len, 
                &sz, 
                &mode, 
                endCh, 
                &breakFlag, 
                &result
            );
            if (breakFlag) {
                break;
            }
        }
    }
    if (ch == EOF) {
        *wasEOF = 1;
    }
    if (mode == WAIT_ONES) {
        fprintf(stderr, "Djarvis: Unexpected EOF during the search <\'>\n");
        fprintf(stderr, "Djarvis: Syntax error \n");
        free(s);
        delete_list(result);
        return NULL;
    }
    if (mode == WAIT_PAIR) {
        fprintf(stderr, "Djarvis: Unexpected EOF during the search <\">\n");
        fprintf(stderr, "Djarvis: Syntax error \n");
        free(s);
        delete_list(result);
        return NULL;
    }
    if (!wasReading) {
        free(s);
        delete_list(result); // tut result na samom dele doljen byt ravnym NULL
        return NULL;
    }
    if (ch == EOF && strlen(s)) { //for files. ...
        result = add_word(result, s, mode != FREE);
    }
    free(s);
    if (!check_brackets(result)) {
        fprintf(stderr, "Djarvis: Brackets, bro..\n");
        fprintf(stderr, "Djarvis: Syntax error \n");
        
        delete_list(result);
        return NULL;
    }
    return result;
    
}
