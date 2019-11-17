#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "../list/list.h"

void delete_tree(ptree t) {
    if (!t) {
        return ;
    }
    delete_list(t->key);
    delete_tree(t->lt);
    delete_tree(t->rt);
    free(t);
}

void print_tree_node(ptree t) {
    if (!t) {
        printf("NULL\n");
        return ;
    }
    print_list(t->key);
}

void print_tree(ptree t) {
    if (!t) {
        return ;
    }
    
    printf("Me:\n");
    print_tree_node(t);
    
    printf("\nleft: \n");
    print_tree_node(t->lt);
    
    
    printf("\nright: \n");
    print_tree_node(t->rt);
    
    printf("\n ============= \n");
    print_tree(t->lt);
    print_tree(t->rt);
}

ptree make_tree(plist p) {
    if (!p) {
        return NULL;
    }
    // A || B && C
    plist e1 = NULL; // ---> B 
    plist b2 = NULL; // ---> C
    plist tmp = p;
    int sum = 0;
    while (tmp && tmp->next) {
        if (tmp->type == BRACKET_OPEN) {
            ++sum;
        } else if (tmp->type == BRACKET_CLOSE) {
            --sum;
        }
        if (sum == 0 && 
            (
                tmp->next->type == LOGIC_AND || 
                tmp->next->type == LOGIC_OR ||
                tmp->next->type == PIPE
            )
        ) {
            e1 = tmp;
            b2 = tmp->next->next;
        }
        tmp = tmp->next;
    }
    ptree res = (ptree) malloc(sizeof (*res));
    if (!e1 && !b2) { // ( ... ) or  ./a arg1 arg2 > b arg3
        plist end = p;
        while (end && end->next && end->next->next) {
            end = end->next;
        }
        /// ./a ar1 ar2 > b ar3
        if (!end->next || p->type != BRACKET_OPEN && end->next->type != BRACKET_CLOSE) {
            res->key = p;
            res->lt = NULL;
            res->rt = NULL;
            return res;
        } else if (p->type == BRACKET_OPEN && end->next->type == BRACKET_CLOSE) {
            printf("DEUUG\n");
            plist arg = p;
            p = p->next;
            arg->next = NULL;
            delete_list(arg);
            
            arg = end->next;
            end->next = NULL;
            arg->next = NULL;//must be NULL before
            delete_list(arg);
            
            return make_tree(p);
        } else {
            printf("Strange..");
            print_list(p);
            exit(1);
        }
    }
    
    res->key = e1->next;
    e1->next = NULL;
    res->key->next = NULL;

    res->lt = make_tree(p);
    res->rt = make_tree(b2);
    return res;
}
