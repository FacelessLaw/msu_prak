#include <stdio.h>
#include <ctype.h>

int readStr(int *len) {
    *len = 0;
    int ch = 0;
    int res = 0;
    while ((ch = getchar()) != EOF) {
        ++(*len);
        if (isspace(ch)) {
            ++res;
        }
        if (ch == '\n') {
            return res;
        }
    }
    return res;
}

int main() {
    int ansCnt = 0;
    int ansLen = 0;
    int curCnt = 0;
    int curLen = 0;
    while ((curCnt = readStr(&curLen))) {
        if (curCnt > ansCnt) {
            ansCnt = curCnt;
            ansLen = curLen;
        }
    }
    printf("%d\n", ansLen);
    return 0;
}