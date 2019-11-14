typedef struct list list;
typedef list * plist;
struct list {
    char * key;
    plist next;
    int type;
};

enum {
    WORD = 0, //./p arg1 arg2 ...
    STREAM = 1, // >> < > &
    PIPE = 2, // >> < > &
    AMP = 3, // >> < > &
    BASH = 4, // && ( ) || , ; 
};

void print_node(plist root);

void print_loop(plist root);

plist del_pid(plist root, int pid, plist * node);

plist add_word(plist root, char *key, int type);
/*
    if root is NULL create new plist el and return pointer
    else return root
    add key to end of list
*/

plist next_cmd(plist root); 
/*
A | B | C && D

return A
return B
return C

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