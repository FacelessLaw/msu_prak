typedef struct list list;
typedef list * plist;

enum {
    WORD_MODE = 0,
    BASH_MODE = 1,    
};

plist add_word(plist root, char *key, int type);
/*
    if root is NULL create new plist el and return pointer
    else return root
    add key to end of list
*/

void delete_list(plist root);
/*
    free mem
*/

void print_list(plist root);
/*
    hello, cap.
*/

char ** ltoa(plist root);
/*
    hello, cap.
*/