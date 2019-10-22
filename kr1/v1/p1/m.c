#include <stdio.h>

int readStr() {
    int ch = 0;
    int res = 0;
    while ((ch = getchar()) != EOF) {
        ++res;
        if (ch == '\n') {
            return res;
        }
    }
    return res;
}

int main() {
    int maxSize = 0;
    int curSize = 0;
    while ((curSize = readStr())) {
        if (curSize > maxSize) {
            maxSize = curSize;
        }
    }
    printf("%d\n", maxSize);
    return 0;
}