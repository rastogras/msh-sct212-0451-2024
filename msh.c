#include <fcntl.h>
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
        while (waitpid(-1, NULL, WNOHANG) > 0) {
        }

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

        int bg = 0;
        if (strcmp(args[i - 1], "&") == 0) {
            bg = 1;
            args[--i] = NULL;
            if (i == 0) {
                continue;
            }
        }

        char *outfile = NULL;
        for (int j = 0; j < i; j++) {
            if (strcmp(args[j], ">") == 0) {
                outfile = args[j + 1];
                args[j] = NULL;
                break;
            }
        }

        if (args[0] == NULL) {
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
            if (outfile != NULL) {
                int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("open");
                    exit(1);
                }
                if (dup2(fd, STDOUT_FILENO) < 0) {
                    perror("dup2");
                    close(fd);
                    exit(1);
                }
                close(fd);
            }
            execvp(args[0], args);
            perror(args[0]);
            exit(127);
        } else {
            if (bg) {
                printf("[%d]\n", pid);
                status = 0;
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
    }

    free(line);
    return 0;
}
