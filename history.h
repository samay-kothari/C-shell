#ifndef HISTORY
#define HISTORY

#include"global.h"
#include"headers.h"

int min(int x, int y){
    if(x<y){
        return x;
    }else{
        return y;
    }
}

void history(){
    if(argumentNumber > 2){
        printf("Too many arguments for history command.\n");
        return;
    }
    char* path = (char *)malloc(VARIABLE_LENGTH*sizeof(char));
    strcpy(path, originalPath);
    strcat(path, "/.shell_history.txt");
    int reader = open(path, O_RDONLY);
    char fileContent[20*VARIABLE_LENGTH];
    int historyNumber = 10;
    if(argumentNumber == 2){
        historyNumber = atoi(argumentInput[1]);
        if(historyNumber > 10 || historyNumber <= 0){
            printf("Invalid number of history requested.\n");
            return;
        }
    }
    read(reader, fileContent, sizeof(fileContent));
    close(reader);

    char **commands = (char **)malloc(20 * MAX_PATH_DEPTH * sizeof(char *));

    char *tokenize = strtok(fileContent, "\n");
    int index = 0;
    while(tokenize!=NULL){
        commands[index] = tokenize;
        tokenize = strtok(NULL, "\n");
        index++;
    }
    commands[index] = NULL;
    historyNumber = min(index, historyNumber);
    for(int i = index - historyNumber; i < index; i++){
        printf("%s\n", commands[i]);
    }
    return;
}
void writeHistory(char* command){
    char* path = (char *)malloc(VARIABLE_LENGTH*sizeof(char));
    strcpy(path, originalPath);
    strcat(path, "/.shell_history.txt");
    int reader = fopen(path, "r");
    char fileContent[20*VARIABLE_LENGTH];
    char newFileContent[20*VARIABLE_LENGTH];
    read(reader, fileContent, sizeof(fileContent));
    close(reader);

    char **commands = (char **)malloc(20 * MAX_PATH_DEPTH * sizeof(char *));

    char *tokenize = strtok(fileContent, "\n");
    int index = 0;
    while(tokenize!=NULL){
        commands[index] = tokenize;
        tokenize = strtok(NULL, "\n");
        index++;
    }
    commands[index] = NULL;
    if(index < 20){
        if(index > 0){
            if(strcmp(commands[index - 1], command) != 0){
                commands[index] = command;
            }
        }
        else{
            // strcpy(commands[index], command);
            commands[index] = command;
        }
        for(int i = 0; i < index + 1 ; i++){
            strcat(newFileContent, commands[i]);
            strcat(newFileContent, "\n");
        }
    }else{
        if(strcmp(commands[index - 1], command) != 0){
            strcpy(commands[index], command);
        }
        for(int i = 1; i < index + 1 ; i++){
            strcat(newFileContent, commands[i]);
            strcat(newFileContent, '\n');
        }
    }
    int writer = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    write(writer, newFileContent, strlen(newFileContent)*sizeof(char));
    close(writer);

}

#endif