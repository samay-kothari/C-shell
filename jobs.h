#include "headers.h"
#include "global.h"


void jobs(){
    int flagr = 0;
    int flags = 0;
    int lineNumber = 1;
    for(int i = 1 ; i < argumentNumber ; i++){
        if(argumentInput[i][0] == '-'){
            if(argumentInput[i][1] == 'r'){
                flagr = 1;
            }
            else if(argumentInput[i][1] == 's'){
                flags = 1;
            }
            else{
                printf("Not valid flag for jobs.\n");
            }
        }
    }
    for(int i = 0; i < processCommand ; i++){
        if(getpgid(processesPID[i]) > 0){
            char* processFile = (char *)malloc(VARIABLE_LENGTH * sizeof(char));
            strcat(processFile, "/proc/");
            char stringPID[256];
            int intPID = processesPID[i];
            int place = 0;
            char** sepratedValues = (char **)malloc(VARIABLE_LENGTH*sizeof(char *));
            char line[VARIABLE_LENGTH];
            char* tokenized;
            while(intPID > 0){
                stringPID[place] = 48 + intPID%10;
                intPID/=10;
                place++;
            }
            for(int a = 0; a < place/2 ; a++){
                char temp = stringPID[place - a - 1];
                stringPID[place - a - 1] = stringPID[a];
                stringPID[a] = temp;
            }
            stringPID[place] = '\0';
            strcat(processFile, stringPID);
            strcat(processFile, "/stat");

            FILE *reader = fopen(processFile, "r");
            fgets(line, VARIABLE_LENGTH, reader);
            int k = 0;
            tokenized = strtok(line, " ");
                while(tokenized!=NULL){
                    sepratedValues[k] = tokenized;
                    tokenized = strtok(NULL, " ");
                    k++;
            }
            if (strcmp(sepratedValues[2], "T") == 0 && (flags || (!flags && !flagr)))
                printf("[%d] Stopped %s [%d]\n", i + 1, processesNames[i], processesPID[i]);
            else if(flagr || (!flagr && !flags))
                printf("[%d] Running %s [%d]\n", i + 1, processesNames[i], processesPID[i]);
        }
    }

}