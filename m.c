#include "tools/wc/wc.h"
#include "tools/cat/cat.h"
#include "tools/my_string/my_string.h"
#include "tools/my_string/my_string.h"
#include "tools/shl_sort/shl_sort.h"

#include <string.h>
#include <stdio.h>

const char * WC = "wc";
const char * CAT = "cat";
const char * STR = "my_string";
const char * SHL_SORT = "shl_sort";

enum {
    COMMAND=1
};


int main(int argc, char **argv) {
    if (argc < 2) {
        return 0; //todo
    }
    if (!strcmp(argv[COMMAND], CAT)) {
        cat();
    } else if (!strcmp(argv[COMMAND], WC)) {
        wc();
    } else if (!strcmp(argv[COMMAND], STR)) {
		str();
	} else if (!strcmp(argv[COMMAND], SHL_SORT)) {
        shl_sort();
    }
    return 0; 
}
