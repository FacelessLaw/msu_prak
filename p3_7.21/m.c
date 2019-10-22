#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    A_CNT = 2,
    A_FILE_R = 1,
};

typedef struct tree * ptree;

struct tree{
    char *key;
    int cnt;
    ptree l, r;
};

ptree insert(ptree root, char *key) {
    if (!root) {
        root = (ptree) malloc(sizeof(*root));
        (root->l) = (root->r) = NULL;
        root->key = (char *) realloc(NULL, sizeof(*key) * (strlen(key) + 1));
        strcpy(root->key, key);
        root->cnt = 1;
        return root;
    }
    if (strcmp(root->key, key) > 0) {
        root->l = insert(root->l, key);
    } else if (strcmp(root->key, key) < 0)  {
        root->r = insert(root->r, key);
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
    free(root->key);
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

char *findMax(ptree root, int *cnt) {
    if (!root) {
        *cnt = 0;
        return NULL;
    }
    *cnt = root->cnt;
    char * resStr = root->key;
    
    int lCnt = 0;
    char * lStr = findMax(root->l, &lCnt);
    
    int rCnt = 0;
    char * rStr = findMax(root->r, &rCnt);
    
    if (lCnt > *cnt) {
        resStr = lStr;
        *cnt = lCnt; 
    }
    if (rCnt > *cnt) {
        resStr = rStr;
        *cnt = rCnt; 
    }
    return resStr;
}

void print(ptree root) {
    if (!root) { return; }
    print(root->l);
    printf("CNT::%d \n STR::%s\n\n", root->cnt, root->key);
    print(root->r);
}

int main(int argc, char *argv[]) {
    if (argc != A_CNT) {
        fprintf(stderr, "\n I need %d args \n", A_CNT);
        exit(1);
    }
    FILE *fr = fopen(argv[A_FILE_R], "r");
    if (!fr) {
        fprintf(stderr, "\n I can't open file: %s  \n", argv[A_FILE_R]);
        exit(1);
    }
    ptree root = NULL;
    char *s = NULL;
    
    while ((s = readStr(fr))) {
        root = insert(root, s);
        free(s);
        s = NULL;
    }
    fclose(fr);
    int cnt = 0;
    char * resStr = findMax(root, &cnt);
    
    printf(" CNT::%d \n STR::%s\n", cnt, resStr);
    //print(root);
    free_tree(root);
    return 0;
}