#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>

char ** addNewStr(char **a, int id, int *len) {
    char ** res = NULL;
    if (id == *len) {
        int newSz = 0;
        if (*len == 0) {
            newSz = 1;
            res = (char **)malloc(sizeof(*a));
        } else {
            newSz = *len * 2;
            res = (char **)realloc(a, sizeof(*a) * (newSz));
        }
        if (res) {
            *len = newSz;
        } else {
            return res;
        }
    } else {
        res = a;
    }
    res[id] = NULL;
    return res;
}

char * addNewCh(char *s, int ch, int *id, int *len) {
    char * res = NULL;
    if (*id >= (*len) - 1) {
        int newSz = 0;
        if (*len == 0) {
            newSz = 1;
            res = (char *) malloc(2 * sizeof(*s));
        } else {
            newSz = *len * 2;
            res = (char *) realloc(s, sizeof(*s) * (newSz));
        }
        if (res) {
            *len = newSz;
        } else {
            return res;
        }
    } else {
        res = s;
    }
    res[(*id)++] = ch;
    res[*id] = '\0';
    return res;
}

int main() {
    char **a;
    int idA = 0, lenA = 0;
    int idS = 0, lenS = 0;
    int ch;
    char **tmp = addNewStr(a, idA, &lenA);
    if (tmp) {
        a = tmp;
    } else {
        return 0;
    }
    while ((ch = getchar()) != EOF) {
        int realCh = (ch == '\n' ? '\0' : ch);
        char *str = addNewCh(a[idA], realCh, &idS, &lenS);
        if (str) {
            a[idA] = str;
        } else {
            printf("Error in adding char to str\n");
            break;
        }

        if (ch == '\n') {
            ++idA;
            lenS = 0;
            idS = 0;
            char ** tmpAr = addNewStr(a, idA, &lenA);
            if (tmpAr) {
                a = tmpAr;
            } else {
                printf("Error in creating new str\n");
                break;
            }
        }
    }
    char *str = addNewCh(a[idA], '\0', &idS, &lenS);
    if (str) {
        a[idA] = str;
    } else {
        printf("Error in adding char to str\n");
    }

    for (int i = 0; i < idA; ++i) {
        for (int j = i + 1;  j < idA; ++j) {
            if (strcmp(a[i], a[j]) > 0) {
                char * tmp = a[i];
                a[i] = a[j];
                a[j] = tmp;
            }
        }
    }
    for (int i = 0; i < idA; ++i) {
        printf("%s\n", a[i]);
        free(a[i]);
    }
    return 0;
}
