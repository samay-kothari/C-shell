#include"headers.h"
#include"global.h"

void changeInputFile(){
    int index = 0;
    int test = 0;
    for(int i = 0; i < argumentNumber ; i++){
        if(argumentInput[i][0] == '<' && i >= 1){
            index = i;
            test = 1;
            break;
        }
    }
    if(test == 0){
        return;
    }
    char *fileName = (char *)malloc(VARIABLE_LENGTH * sizeof(char));
    strcpy(fileName, argumentInput[index+1]);
    for(int i = index ; i < argumentNumber - 2 ; i++){
        argumentInput[i] = argumentInput[i + 2];
    }
    argumentInput[argumentNumber-1] = NULL;
    argumentInput[argumentNumber-2] = NULL;
    argumentNumber-=2;

    int fd = open(fileName, O_RDONLY);
    if(fd <= 0){
        perror("File Error: ");
        return;
    }
    OriStdIn = dup(0);
    dup2(fd, 0);
    isStdInChanged = 1;
    return;
}

void resetInputFile(){
    dup2(OriStdIn, 0);
    isStdInChanged = 0;
    return;
}

void changeOutputFile(){
    int index = 0;
    int overwrite = 0;
    int test = 0;
    for(int i = 0; i < argumentNumber ; i++){
        if(argumentInput[i][0] == '>'){
            index = i;
            if(strcmp(argumentInput[i], ">>")==0){
                overwrite = 0;
            }else if(strlen(argumentInput[i])==1){
                overwrite = 1;
            }else{
                return;
            }
            test = 1;
            break;
        }
    }
    if(test == 0){
        return;
    }
    char *fileName = (char *)malloc(VARIABLE_LENGTH * sizeof(char));
    strcpy(fileName, argumentInput[index+1]);
    for(int i = index ; i < argumentNumber - 2 ; i++){
        argumentInput[i] = argumentInput[i + 2];
    }
    argumentInput[argumentNumber-1] = NULL;
    argumentInput[argumentNumber-2] = NULL;
    argumentNumber-=2;
    int fd;
    if (overwrite == 1){
        fd = open(fileName, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    }else{
        fd = open(fileName, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    }
    if (fd == -1)
    {
        perror("File error");
        return;
    }
    OriStdOut = dup(1);
    dup2(fd, 1);
    isStdOutChanged = 1;
}
void resetOutputFile(){
    dup2(OriStdOut, 1);
    isStdOutChanged = 0;
}