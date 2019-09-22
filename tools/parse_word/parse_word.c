#include <stdio.h>
#include <ctype.h>
#include <string.h>

const char sep[4] = {
    ' ', '\n', ',', '\0'
};

void parse_word() {
    int ch;
    int last_was_sep = 1;
    while ((ch = getchar()) != EOF) {
        if (strchr(sep, ch)) {
            if (!last_was_sep) 
                putchar('\n');
            last_was_sep = 1;
        } else {
            last_was_sep = 0;
            putchar(ch);
        }
    }
    putchar('\n');
}