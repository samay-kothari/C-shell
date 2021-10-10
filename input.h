#include"headers.h"
#include"global.h"
#include"builtin.h"
#include"lsHandler.h"
#include"pinfoHandler.h"
#include"unknownCommands.h"
#include"history.h"
#include"redirection.h"
#include"jobs.h"
#include"signalHandler.h"
#include"bgHandler.h"
#include"fgHandler.h"

char *readInput(){
    char *str = (char *)malloc(VARIABLE_LENGTH*sizeof(char));
    if(!str){
        memoryAllocationError();
    }

    size_t len = VARIABLE_LENGTH;
    getline(&str, &len, stdin);

    return str;
}

char **tokenizeInput(char *command){
    argumentInput = (char **)malloc(MAX_ARGUMENTS*sizeof(char *));
    if(!argumentInput){
        memoryAllocationError();
    }
    int k = 0;
    char* tokenized;
    tokenized = strtok(command, DELIMITER);
    while(tokenized!= NULL){
        argumentInput[k++] = tokenized;
        tokenized = strtok(NULL, DELIMITER);
    }
    argumentInput[k] = NULL;
    argumentNumber = k;
    return argumentInput;
}

void preCommand();

void checkCommand(int* flag){
    if(strcmp(argumentInput[0], "echo") == 0){
        echo();
    }
    else if(strcmp(argumentInput[0], "cd") == 0){
        cd();
    }
    else if(strcmp(argumentInput[0], "pwd") == 0){
        pwd();
    }
    else if(strcmp(argumentInput[0], "ls") == 0){
        ls();
    }
    else if(strcmp(argumentInput[0], "pinfo") == 0){
        pinfo();
    }
    else if(strcmp(argumentInput[0], "jobs") == 0){
        jobs();
    }
    else if(strcmp(argumentInput[0], "sig") == 0){
        sig();
    }
    else if(strcmp(argumentInput[0], "bg") == 0){
        bg();
    }
    else if(strcmp(argumentInput[0], "fg") == 0){
        fg();
    }
    else if(strcmp(argumentInput[0], "exit") == 0){
        *flag = 1;
        return;
    }
    else if(strcmp(argumentInput[0], "repeat") == 0){
        int times = argumentInput[1][0]-'0';
        char* nextCommand = (char *)malloc(VARIABLE_LENGTH*sizeof(char));
        char* storageCommand = (char *)malloc(VARIABLE_LENGTH*sizeof(char));
        for(int k = 2; k < argumentNumber ; k++){
            strcat(nextCommand, argumentInput[k]);
            strcat(nextCommand, " ");
        }
        strcpy(storageCommand, nextCommand);
        for(int j = 0; j < times; j++){
            strcpy(nextCommand, storageCommand);
            preCommand(nextCommand, &flag);
        }
    }
    else if(strcmp(argumentInput[0], "history") == 0){
        history();
    }
    else{
        manageUnknownCommands();
    }
    if(isStdInChanged){
        resetInputFile();
    }
    if(isStdOutChanged){
        resetOutputFile();
    }
    return;
}
void doPiping(int in, int out, int* flag)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        argumentInput[argumentNumber] = NULL;
        if (in != 0)
        {
            dup2(in, 0);
            close(in);
        }
        if (out != 1)
        {
            dup2(out, 1);
            close(out);
        }
        checkCommand(&flag);
        exit(0);
    }
    waitpid(pid, NULL, WUNTRACED);
}
void checkPipes(int* flag){
    int fd[2];
    int input = 0;
    int tempArgumentNumber = argumentNumber;
    char** tempArgumentInput = argumentInput;
    argumentNumber = 1;
    for(int i = 1 ; i < tempArgumentNumber ; i++){
        printf("%d %s\n", argumentNumber, argumentInput[0]);
        if(strcmp(tempArgumentInput[i], "|") == 0){
            pipe(fd);
            doPiping(input, fd[1], flag);
            close(fd[1]);
            input = fd[0];
            argumentInput = argumentInput + argumentNumber;
            argumentInput++;
            argumentNumber = 1;
            i++;
        }else{
            argumentNumber++;
        }
    }
    printf("%d %s\n", argumentNumber, argumentInput[0]);
    doPiping(input, 1, flag);
    argumentNumber = tempArgumentNumber;
    argumentInput = tempArgumentInput;
}
void preCommand(char* command, int* flag){
    writeHistory(command);
    tokenizeInput(command);
    if(strcmp(argumentInput[0], "exit") == 0){
        *flag = 1;
        return;
    }
    changeInputFile();
    changeOutputFile();
    checkPipes(flag);
}

void seprateMultipleCommand(char *command, int* flag){
    sepratedCommands = (char **)malloc(MAX_COMMANDS*sizeof(char *));
    if(!sepratedCommands){
        memoryAllocationError();
    }
    int k = 0;
    char* tokenized;
    tokenized = strtok(command, ";");
    while(tokenized!=NULL){
        sepratedCommands[k++] = tokenized;
        tokenized = strtok(NULL, ";");
    }
    sepratedCommands[k] = NULL;
    commandNumber = k;
    for (int i = 0; i < commandNumber; i++){
        preCommand(sepratedCommands[i], flag);
    }
    return;
}


