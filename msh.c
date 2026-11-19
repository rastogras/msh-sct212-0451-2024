#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

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

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
        } else if (pid == 0) {
            execvp(args[0], args);
            perror(args[0]);
            exit(127);
        } else {
            if (waitpid(pid, NULL, 0) < 0) {
                perror("waitpid");
            }
        }
    }

    free(line);
    return 0;
}
