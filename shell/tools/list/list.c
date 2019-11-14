#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

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

plist del_pid(plist root, int pid, plist * node) {
    *node = NULL;
    if (!root) {
        return NULL;
    }
    plist fs = root;
    plist sc = root->next;
    if (fs->type == pid) {
        *node = fs;
        return sc;
    }
    while (sc) {
        if (sc->type == pid) {
            *node = sc;
            fs->next = sc->next;
            return root;
        }
        fs = sc;
        sc = sc->next;
    }
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

void print_node(plist root) {
    printf("proc: [%d] was killed \t %s \n\n", root->type, root->key);
}

void print_loop(plist root) {
    while (root) {
        //printf("proc: [%d] was killed \t %s \n", root->type, root->key);
        print_node(root);
        root = root->next;
    }
}

void print_list(plist root) {
    while (root) {
        printf(" ---> %s <--- <%d> \n", root->key, root->type);
        root = root->next;
    }
}

int check_brackets(plist res) {
    int sum = 0;
    while (res) {
        if (res->type == BASH) {
            if (!strcmp(res->key, ")")) {
                --sum;
            } else if (!strcmp(res->key, "(")) {
                ++sum;
            }
        }
        if (sum < 0) {
            return 0;
        }
        res = res->next;
    }
    return sum == 0;
}

/*
plist next_cmd(plist root) {
    while (root && root->) {
        
    }
} */
