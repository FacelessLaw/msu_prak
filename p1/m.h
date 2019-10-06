#include <stdio.h>
#include <stdlib.h>

void *
allocate(void *source, int curr_it, int *last_it, int size_of_block) {
    // Function, that allocate memory
    void *res = source;
    if (curr_it + 1 >= *last_it) {
        int new_size = (*last_it != 0) * (*last_it) * 2  + (*last_it == 0) * 2;
        res = (void *) realloc(source, new_size * size_of_block);
        if (!res) {
            fprintf(stderr, "Mem limit");
            exit(1);
        }
        *last_it = new_size;
    }
    return res;
}

char **
add_string(char **source, int curr_it, int *last_it) {
    char **res = (char **) allocate(source, curr_it, last_it, sizeof(*source));
    res[curr_it] = NULL;
    return res;
}

char *
add_char(char *source, int symb, int curr_it, int *last_it) {
    char *res = (char *) allocate(source, curr_it, last_it, sizeof(*res));
    res[curr_it] = symb;
    res[curr_it + 1] = '\0';
    return res;
}