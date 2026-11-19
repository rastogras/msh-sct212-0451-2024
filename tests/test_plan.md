# Test Plan for msh

| Input | Expected behaviour | Actual result |
| :--- | :--- | :--- |
| `(blank line)` | New prompt, no error | Displayed a new `msh[0]>` prompt with no error |
| `echo hello world` | Prints text; status 0 | Printed `hello world` and showed `msh[0]>` |
| `ls /nonexistent` | Error from ls; non-zero status | Printed `ls: cannot access '/nonexistent': No such file or directory` and showed `msh[2]>` |
| `nosuchcommand` | Error message; status 127 | Printed `nosuchcommand: No such file or directory` and showed `msh[127]>` |
| `cd /tmp` then `pwd` | Prints /tmp | Changed directory and `pwd` printed `/tmp` |
| `sleep 3 &` | Prompt returns at once; PID shown | Printed `[18674]` and returned immediately to `msh[0]>` |
| `echo hi > f.txt` then `cat f.txt` | File contains hi | Created `f.txt` with mode 0644 containing `hi` |
| `sleep 100` then `Ctrl-C` | Sleep stops; shell survives; status 130 | Interrupted `sleep`, kept shell alive, and showed `msh[130]>` |
| `Ctrl-D` at the prompt | Shell exits cleanly | Printed newline, freed line buffer, and exited with code 0 |
| `cd` (with no arguments) | Changes directory to `$HOME` | Changed working directory to `/home/kali` and showed `msh[0]>` |
| `help` | Prints list of built-in commands | Printed `msh built-ins: cd [dir], help, exit` and showed `msh[0]>` |
| `   ls   -l   ` (extra spaces/tabs) | Ignores extra whitespace and runs `ls -l` | Listed directory contents properly and showed `msh[0]>` |
