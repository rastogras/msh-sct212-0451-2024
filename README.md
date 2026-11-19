# msh - Minimal Unix Command-Line Shell

- **Name:** Roy Kiptoo
- **Student ID:** SCT212-0451/2024

## How to Build and Run

    make
    ./msh

To remove the compiled binary:

    make clean

## Features Implemented

| Task | Description | Status |
| :--- | :--- | :--- |
| 1 | Prompt display, `getline()`, clean exit on `Ctrl-D` | Complete |
| 2 | Tokenizing input on spaces and tabs into `NULL`-terminated `argv` | Complete |
| 3 | External commands via `fork()`, `execvp()`, and `waitpid()` | Complete |
| 4 | Built-in commands: `cd [dir]` (default `$HOME`), `help`, and `exit` | Complete |
| 5 | Previous command exit status in prompt (`msh[status]>`) | Complete |
| 6 | Background execution (`&`) and zombie reaping with `WNOHANG` | Complete |
| 7 | Output redirection (`>`) in child using `open()`, `dup2()`, `close()` | Complete |
| 8 | Handling `SIGINT` (`Ctrl-C`) without terminating the shell | Complete |

## Design Explanation

When `msh` reads a non-empty command line that is not a built-in, it runs the `fork()` / `execvp()` / `waitpid()` cycle. First, `fork()` creates a duplicate child process. Inside the child (`pid == 0`), if output redirection (`>`) was requested, the child opens the target file with `O_WRONLY | O_CREAT | O_TRUNC` (mode `0644`), redirects standard output to that file descriptor using `dup2()`, and closes the original descriptor. It then calls `execvp()`, which replaces the child's memory image with the requested program. If `execvp()` fails, the child prints an error using `perror()` and exits with status `127`. For foreground jobs, the parent calls `waitpid()` to pause until the child finishes and extracts its exit code using `WIFEXITED` and `WEXITSTATUS` (or `128 + WTERMSIG` if killed by a signal).

`cd` and `exit` must be built-ins rather than external programs because each Unix process has its own current working directory and execution state in the kernel. If `cd` ran inside a forked child process, `chdir()` would only change the child's directory before the child exited, leaving the parent shell's directory unchanged. Similarly, an external `exit` binary would only terminate itself rather than the shell loop.

To avoid zombie processes when background jobs (`&`) finish, the parent does not block immediately. Instead, before displaying each new prompt, `msh` runs `while (waitpid(-1, NULL, WNOHANG) > 0)` to reap any terminated background children without blocking.

## Known Limitations

- Input redirection (`<`) and pipes (`|`) are not supported.
- Quoted strings containing spaces are split on whitespace rather than kept as one argument.
- Only one output redirection (`>`) per command is supported.

## Sources and AI-Tool Declaration

- Linux manual pages (`man 2 fork`, `man 3 execvp`, `man 2 waitpid`, `man 2 dup2`, `man 3 getline`) for system call signatures and flags.
- AI assistance was used to clarify POSIX header requirements, verify signal handling edge cases, and structure the documentation.
