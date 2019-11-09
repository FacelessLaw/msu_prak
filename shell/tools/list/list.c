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

plist create_node(char *key, int type) {
    plist root = (plist) malloc(sizeof(*root));
    root->next = NULL;
    root->type = type;
    root->key = (char *) malloc(sizeof(*key) * (strlen(key) + 1));
    strcpy(root->key, key);
    return root;
}

plist add_word(plist root, char *key, int type) {
    if (!root) {
        return create_node(key, type);
    }
    plist it = root;
    while (it->next) {
        it = it->next;
    }
    it->next = create_node(key, type);
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
        printf(" ---> %s <--- <%d> \n", root->key, root->type);
        root = root->next;
    }
}