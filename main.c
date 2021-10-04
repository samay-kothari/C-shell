#include"headers.h"
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
    while(1 && !flag){
        initialisePrompt();
        input = (char *)malloc(VARIABLE_LENGTH * sizeof(char));
        printf("%s ", prompt);
        fflush(stdout);
        strcpy(input, readInput());
        if(strlen(input) > 0 && (strcmp(input, "\n")!=0)){
            seprateMultipleCommand(input, &flag);
        }
        else{
            continue;
        }
    }
    return 0;
}