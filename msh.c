#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char *line = NULL;
    size_t len = 0;
    char *args[64];

    while (1) {
        printf("msh> ");
        fflush(stdout);

        if (getline(&line, &len, stdin) == -1) {
            printf("\n");
            break;
        }

        int i = 0;
        char *tok = strtok(line, " \t\n");
        while (tok != NULL && i < 63) {
            args[i++] = tok;
            tok = strtok(NULL, " \t\n");
        }
        args[i] = NULL;

        if (i == 0) {
            continue;
        }
    }

    free(line);
    return 0;
}
