typedef struct list list;
typedef list * plist;
struct list {
    char * key;
    plist next;
    int type;
};

enum {
    WORD_MODE = 0,
    BASH_MODE = 1,    
};

void print_loop(plist root);

plist del_pid(plist root, int pid, plist * node);

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

int check_brackets(plist res);
/*
())
(()
*/

plist create_node(char *key, int type);
/* create node with key=key, type=type, next=null */