#include <stdio.h>
#include <string.h>
#include "tools/list/list.h"
#include "tools/cmdline/cmdline.h"
#include "tools/runproc/runproc.h"

const char * PARSE = "-p\0";
const char * RUN = "-r\0";
const char * HELP = "-h\0";

int 
main(int argc, char *argv[]) {
    if (argc >= 2) {
        if (!strcmp(argv[1], PARSE)) {
            printf("\n");
            plist cmd = parse_cmd();
            printf("\n");
            print_list(cmd);
            delete_list(cmd);
            return 0;
        } else if (!strcmp(argv[1], RUN)) {
            plist cmd = parse_cmd();
            runproc(cmd);
            delete_list(cmd);
            return 0;
        } else if (!strcmp(argv[1], HELP)) {
            printf("USAGE: ./m [-r, -p, -h]\n\n");
            printf("\t -r \t run command\n\n");
            printf("\t -p \t print parse command\n\n");
            printf("\t -h \t print help\n\n");
            return 0;
        }
    }
    while (1) {
        plist cmd = parse_cmd();
        runproc(cmd);
        delete_list(cmd);
    }
    return 0;
}