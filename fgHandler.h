#include"global.h"
#include"headers.h"

void fg(){
    int jobNumber;
    int pgID;
    int shellPgID;
    if(argumentNumber != 2 ){
        printf("%s: Invalid number of arguments\n", argumentInput[0]);
        return;
    }
    jobNumber = atoi(argumentInput[1]);
    if(jobNumber > processCommand || jobNumber < 1){
        printf("%s: Invalid job number\n", argumentInput[0]);
        return;
    }
    pgID = getpgid(processesPID[jobNumber-1]);
    shellPgID = getpgid(shellPID);
    if(pgID>0){
        currentForegroundProcess = processesPID[jobNumber-1];
        // continuing the stopped process
        kill(pgID, SIGCONT);
        // making the background process group as foreground group
        tcsetpgrp(0, pgID);
        
        // ignoring the signal that corresponds when background process tries to read terminal
        signal(SIGTTIN, SIG_IGN);
        // ignoring the signal that corresponds when background process tries to write to terminal
        signal(SIGTTOU, SIG_IGN);
        
        
        int status;
        // waiting for the process to complete while tracing the child
        int test = waitpid(pgID, NULL, WUNTRACED);
        // resetting the shell as the foreground process
        tcsetpgrp(0, shellPgID);

        // resetting the signals to default
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        currentForegroundProcess = shellPID;
        signal(SIGINT, sigIntHandler);
        signal(SIGTSTP, sigStpHandler);
        return;
    }else{
        printf("%s: Process terminated already\n", argumentInput[0]);
        return;
    }
}