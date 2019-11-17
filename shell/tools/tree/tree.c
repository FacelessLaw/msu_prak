#include "tree.h"
#include "stdlib.h"

ptree make_tree(plist p) {
    ptree res = (ptree) malloc(sizeof (*res));
    // A || B && C
    plist e1 = p; // ---> B 
    plist b2 = p; // ---> C
    plist tmp = p;
    while (tmp && tmp->next) {
        //if (tmp->next->type == BASH) {

        //}
    }
    return res;
}