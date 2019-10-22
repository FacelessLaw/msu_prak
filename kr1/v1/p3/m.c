#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree * ptree;

struct tree{
    int key;
    ptree l, r;
};

ptree insert(ptree root, int key) {
    if (!root) {
        root = (ptree) malloc(sizeof(*root));
        root->l = root->r = NULL;
        root->key = key;
    }
    if (root->key > key) {
        root->l = insert(root->l, key);
    } else if (root->key < key) {
        root->r = insert(root->r, key);
    }
    return root;
}

void free_tree(ptree root) {
    if (root) {
       free_tree(root->l);
       free_tree(root->r);
    }
    free(root);
}

int print(ptree root, int h) {
    if (!root) {
        return 0;
    }
    if (h == 0) {
        printf("%d ", root->key);
        return 1;
    }
    int r1 = print(root->l, h - 1);
    int r2 = print(root->r, h - 1);
    return r1 || r2;
}



int main(int argc, char *argv[]) {
    ptree root = NULL;
    int i;
    for (i = 1; i < argc; ++i) {
        int key = atoi(argv[i]);
        root = insert(root, key);
    }
    i = 0;
    while (print(root, i++)) {
        printf("\n");
    }
    free_tree(root);
    return 0;
}