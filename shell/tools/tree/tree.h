#include "../list/list.h"

typedef struct tree tree;
typedef tree * ptree; 

struct tree {
    plist key;
    ptree lt;
    ptree rt;
};

ptree make_tree(plist p); 

void print_tree(ptree t);

void delete_tree(ptree t);
