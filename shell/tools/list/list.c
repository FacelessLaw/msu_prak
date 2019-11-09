#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list list;
typedef list * plist;

struct list {
    char * key;
    plist next;
    int type;
};

char ** ltoa(plist root) {
    if (!root) {
        return NULL;
    }
    int len = 0, sz = 2;
    char ** res = (char **)realloc(NULL, sizeof(*res) * sz);
    while (root) {
        res[len++] = root->key;
        if (len == sz - 1) {
            sz *= 2;
            res = (char **) realloc(res, sizeof(*res) * sz);
        }
        root = root->next;
    }
    res[len] = NULL;
    return res;
}

plist create_node(char *key) {
    plist root = (plist) malloc(sizeof(*root));
    root->next = NULL;
    root->key = (char *) malloc(sizeof(*key) * (strlen(key) + 1));
    strcpy(root->key, key);
    return root;
}

plist add_word(plist root, char *key) {
    if (!root) {
        return create_node(key);
    }
    plist it = root;
    while (it->next) {
        it = it->next;
    }
    it->next = create_node(key);
    return root;
}

void delete_list(plist root) {
    while (root) {
        plist tmp = root;
        root = root->next;
        free(tmp->key);
        free(tmp);
    }
}

void print_list(plist root) {
    while (root) {
        printf(" ---> %s <--- \n", root->key);
        root = root->next;
    }
}