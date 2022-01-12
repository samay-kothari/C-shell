# Assignment 3 Operating System
## Extending the SHELL
To run the shell do:
```
make
./shell
```

The commands implemented are: 

1. ls(lsHandler.h)
2. echo(builtin.h)
3. cd(builtin.h)
4. pwd(builtin.h)
5. history(history.h)
6. pinfo(pinfoHandler.h)
7. jobs(jobs.h)
8. sig(signalHandler.h)
9. fg(fgHandler.h)
10. bg(bgHandler.h)

* Input.h file manages the input received from the shell and tokenized it to seperate the commands and get space seprated flags and inputs
* global.h have all the constants and global variable that are used in the shell
* headers.h have all the header import required for the functioning.
* initialPrompt.h files prints the initial prompt onto the terminal.
* main.c is the file having the main function and running the entire shell
* unknownCommands.h file is used to run shell commands using execvp that are not implemented.
* & can be used to run process in the background and pinfo can be used to check their status.
* File input.h has functions to maintain the piping functions of the shell.
* File redirection.h maintains the input output redirection of the shell using '>' or '<' symbols.
* signalHandler.h file mangages the sig command.
* file global.h have function that manage the the ctrl+C input and ctrl+Z input to the shell.
* fgHandler.h and bgHandler.h files manage he foreground and background switching of the processes.


Assumptions made:

* Max number of arguments that can be given with a command are 256.
* Maximum length of input can be 2056 character long.
* Max number of commands that can be given at a single time are 256.
* In repeat command the number of repeats that is the first argument after the command name can be 0-9
