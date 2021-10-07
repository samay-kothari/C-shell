#include"headers.h"
#include"global.h"
#include"initialPrompt.h"
#include"input.h"

char *input;

int main(){
    if(getcwd(originalPath, sizeof(originalPath)) == NULL){
        perror("Error in getting shell path: ");
        exit(1);
    }
    shellPID = (int)getpid();
    int flag = 0;
    currentForegroundProcess = shellPID;
    signal(SIGINT, sigIntHandler);
    signal(SIGTSTP, sigStpHandler);
    while(1 && !flag){
        initialisePrompt();
        input = (char *)malloc(VARIABLE_LENGTH * sizeof(char));
        printf("%s ", prompt);
        fgets(input, VARIABLE_LENGTH, stdin);
        if(strlen(input) > 0 && (strcmp(input, "\n")!=0)){
            seprateMultipleCommand(input, &flag);
        }
        else{
            continue;
        }
    }
    return 0;
}