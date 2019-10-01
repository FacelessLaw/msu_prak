#include <stdio.h>
#include <string.h>
#include <ctype.h>

void cat() {
    int ch;
    while ((ch = getchar()) != EOF) {
        putchar(ch);
    }
}
