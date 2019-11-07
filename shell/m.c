#include <stdio.h>
#include "tools/list/list.h"
#include "tools/cmdline/cmdline.h"

int 
main(int argc, char *argv[]) {
    if (argc >= 2) {
        plist cmd = parse_cmd();
        printf("\n");
        print_list(cmd);
        delete_list(cmd);
        return 0;
    }
    while (1) {
        plist cmd = parse_cmd();
        print_list(cmd);
        delete_list(cmd);
    }
    return 0;
}