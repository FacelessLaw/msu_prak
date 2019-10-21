#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    A_CNT = 2,
    A_FILE = 1,
};

typedef struct tree * ptree;

struct tree{
    struct Key {
        char * s;
        int len;
    } key;
    int cnt;
    ptree l, r;
};

ptree insert(ptree root, char *s, int len) {
    if (!root) {
        root = (ptree) malloc(sizeof(*root));
        root->l = root->r = NULL;
        root->key.s = (char *) realloc(NULL, sizeof(*s) * (strlen(s) + 1));
        strcpy(root->key.s, s);
        root->key.len = len;
        root->cnt = 1;
        return root;
    }
    if (root->key.len > len) {
        root->l = insert(root->l, s, len);
    } else if (root->key.len < len) {
        root->r = insert(root->r, s, len);
    } else if (strcmp(root->key.s, s) > 0) {
        root->l = insert(root->l, s, len);
    } else if (strcmp(root->key.s, s) < 0) {
        root->r = insert(root->r, s, len);
    } else {
        root->cnt++;
    }
    return root;
}

void free_tree(ptree root) {
    if (!root) {
        return ;
    }
    free_tree(root->l);
    free_tree(root->r);
    free(root->key.s);
    free(root);
}

char *readStr(FILE *fr) {
    int sz = 2;
    char *s = (char *) realloc(NULL, sizeof(*s) * sz);
    s[0] = 0;
    while (fgets(s + strlen(s), sz - strlen(s), fr)) {
        sz *= 2;
        s = realloc(s, sz);
        if (s[strlen(s) - 1] == '\n') {
            break;
        }
    }
    if (strlen(s) == 0) {
        free(s);
        s = NULL;
    }
    return s;
}

void print(ptree root, FILE *fw) {
    if (!root) { return; }
    print(root->l, fw);
    int i;
    for (i = 0; i < root->cnt; ++i) {
        fputs(root->key.s, fw);
        if (root->key.s[root->key.len - 1] != '\n') {
            fputc('\n', fw);
        }
    }
    print(root->r, fw);
}

int main(int argc, char *argv[]) {
    if (argc != A_CNT) {
        fprintf(stderr, "\n I need %d args \n", A_CNT);
        exit(1);
    }
    FILE *file = fopen(argv[A_FILE], "r");
    if (!file) {
        fprintf(stderr, "\n I can't open file: %s  \n", argv[A_FILE]);
        exit(1);
    }
    ptree root = NULL;
    char *s = NULL;
    
    while ((s = readStr(file))) {
        root = insert(root, s, strlen(s));
        free(s);
        s = NULL;
    }
    fclose(file);
    file = fopen(argv[A_FILE], "w");
    print(root, file);
    fclose(file);
    free_tree(root);
    return 0;
}