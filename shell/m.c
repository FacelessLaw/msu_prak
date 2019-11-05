#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include "tools/list/list.h"
#include "tools/cmdline/cmdline.h"

int 
main(int argc, char *argv[]) {
    plist cmd = parse_cmd();
    print_list(cmd);
    delete_list(cmd);
    return 0;
}