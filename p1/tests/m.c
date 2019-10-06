#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void * 
loc(void *a, int ia, int *la, int sizeVal) {
    void * res = a;
    if (ia + 1 >= *la) {
        int ns = (*la) * 2;
        if (*la == 0) {
            ns = 2;
        }
        res = (void *) realloc(a, ns * sizeVal);
        if (!res) {
            fprintf(stderr, "Memory limit");
            exit(1);
        }
        *la = ns;
    }
    return res;
}

char **
addStr(char **a, int ia, int *la) {
    char ** res = (char **) loc(a, ia, la, sizeof(*a));
    res[ia] = NULL;
    return res;
}

char *
addCh(char *s, int ch, int is, int *ls) {
    char * res = loc(s, is, ls, sizeof(*res));
    res[is] = ch;
    res[is + 1] = '\0';
    return res;
}

int
main(void) {
    char ** a = NULL;
    int la = 0;
    int ia = 0;
    int ls = 0;
    int is = 0;
    int ch;
    a = addStr(a, ia, &la);
    while ((ch = getchar()) != EOF) {
        if (ch == '\n') {
            if (ls == 0) {
                a[ia] = addCh(a[ia], '\0', is, &ls);
            }
            ++ia;
            a = addStr(a, ia, &la);
            ls = is = 0;
        } else {
            a[ia] = addCh(a[ia], ch, is, &ls);
            ++is;
        }
    }
    int i;
    for (i = 0; i < ia; ++i) {
        int j;
        for (j = i + 1; j < ia; ++j) {
            if (strcmp(a[i], a[j]) > 0) {
                char *tmp = a[i];
                a[i] = a[j]; 
                a[j] = tmp;
            }
        }
    }
    for (i = 0; i < ia; ++i) {
        printf("%s\n", a[i]);
        free(a[i]);
    }
    free(a);
    return 0;
}
