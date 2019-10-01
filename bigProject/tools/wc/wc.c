#include <stdio.h>
#include <string.h>
#include <ctype.h>

void wc() {
    int ch;
    int countLines = 0;
    int countWords = 0;
    int countBytes = 0;
    int lastWasSpace = 1;
    while ((ch = getchar()) != EOF) {
        if (isspace(ch)) {
            lastWasSpace = 1;
        } else {
            if (lastWasSpace) {
                ++countWords;
            }
            lastWasSpace = 0;
        }
        if (ch == '\n') {
            ++countLines;
        } 
        ++countBytes;
    }
    printf("%d %d %d\n", countLines, countWords, countBytes);
}
