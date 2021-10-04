#include"headers.h"
#include"global.h"


void startForegroundProcess(){
    pid_t flag = fork();
    if(flag == 0){
        if(execvp(argumentInput[0], argumentInput) < 0){
            printf("%s: command not found\n", argumentInput[0]);
            exit(1);
        }
        exit(0);
    }else if(flag < 0){
        printf("%s: unable to fork\n", argumentInput[0]);
    }else{
        int status;
        // waiting for the child process to end, so running in foreground
        waitpid(flag, &status, 0);
    }
}

void startBackgroundProcess(){
    pid_t flag = fork();
    // printf("%d lol\n", flag);
    if(flag < 0){
        printf("%s: unable to fork\n", argumentInput[0]);
        return;
    }
    else if(flag > 0){
        // for the parent process
        processesPID[processCommand] = flag;
        strcpy(processesNames[processCommand], argumentInput[0]);
        printf("[%d] %d\n", processCommand+1, flag);
        processCommand++;

    }else{
        char **newargs = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
        int idx = 0;
        for (int a = 0; a < argumentNumber; a++)
        {
            if (strcmp(argumentInput[a], "&") == 0)
                continue;
            newargs[idx++] = argumentInput[a];
        }
        newargs[idx] = NULL;
        if (setpgid(getpid(), 0) == -1)
        {
            perror(newargs[0]);
            exit(1);
        }
        if (execvp(newargs[0], newargs) < 0)
        {
            printf("%s: command not found\n", newargs[0]);
            exit(1);
        }
        exit(0);
    }
}

void endingProcess()
{
    for (int a = 0; a < processCommand; a++)
    {
        int status;
        int test = waitpid(processesPID[a], &status, WNOHANG | WUNTRACED);
        if (test > 0)
        {
            printf("[%d] %s with pid %d exited ", a + 1, processesNames[a], processesPID[a]);
            if (WIFEXITED(status) > 0)
                printf("normally");
            else
                printf("abnormally");
            printf("\n");
        }
    }
}

void manageUnknownCommands()
{
    signal(SIGCHLD, endingProcess);
    for (int a = 1; a < argumentNumber; a++)
        if (strcmp(argumentInput[a], "&") == 0)
        {
            startBackgroundProcess();
            return;
        }
    startForegroundProcess();
}