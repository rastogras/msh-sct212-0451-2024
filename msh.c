#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    char *line = NULL;
    size_t len = 0;
    char *args[64];
    int status = 0;

    while (1) {
        printf("msh[%d]> ", status);
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

        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        if (strcmp(args[0], "cd") == 0) {
            char *dir = args[1] ? args[1] : getenv("HOME");
            if (dir == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
                status = 1;
            } else if (chdir(dir) != 0) {
                perror("cd");
                status = 1;
            } else {
                status = 0;
            }
            continue;
        }

        if (strcmp(args[0], "help") == 0) {
            printf("msh built-ins: cd [dir], help, exit\n");
            status = 0;
            continue;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            status = 1;
        } else if (pid == 0) {
            execvp(args[0], args);
            perror(args[0]);
            exit(127);
        } else {
            int wstatus;
            if (waitpid(pid, &wstatus, 0) < 0) {
                perror("waitpid");
                status = 1;
            } else if (WIFEXITED(wstatus)) {
                status = WEXITSTATUS(wstatus);
            } else if (WIFSIGNALED(wstatus)) {
                status = 128 + WTERMSIG(wstatus);
            }
        }
    }

    free(line);
    return 0;
}
