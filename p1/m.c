#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "m.h"

int
main(void) {
    char **text = NULL;
    int absolute_last_it = 0;
    int absolute_it = 0;
    int curr_last_it = 0;
    int curr_it = 0;
    int ch;
    text = add_string(text, absolute_it, &absolute_last_it);
    while ((ch = getchar()) != EOF) {
        if (ch == '\n') { //in case we got full new string
            if (curr_last_it == 0) {
                text[absolute_it] = add_char(text[absolute_it], '\0', curr_it, &absolute_last_it);
            }
            ++absolute_it;
            text = add_string(text, absolute_it, &absolute_last_it);
            curr_it = curr_last_it = 0;
        } else { //in case we still scanning new string
            text[absolute_it] = add_char(text[absolute_it], ch, curr_it, &curr_last_it);
            ++curr_it;
        }
    }
    absolute_it += (curr_last_it != 0); //in case we have not met last new-line symb
    for (int i = 0; i < absolute_it; ++i) { //sorting
        for (int j = i + 1; j < absolute_it; ++j) {
            if (strcmp(text[i], text[j]) > 0) { //swap strings
                char *tmp = text[i];
                text[i] = text[j]; 
                text[j] = tmp;
            }
        }
    }
    for (int i = 0; i < absolute_it; ++i) {
        printf("%s\n", text[i]);
        free(text[i]);
    }
    free(text);
    return 0;
}
