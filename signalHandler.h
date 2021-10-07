#include"headers.h"
#include"global.h"

void sig(){
    if(argumentNumber != 3){
        printf("Invalid Number of arguments for sig command\n");
        return;
    }  
    int processIndex = atoi(argumentInput[1]);
    int signalValue = atoi(argumentInput[2]);
    if(signalValue>31 || signalValue<1){
        printf("Invalid sigal value\n");
        return;
    }
    if(processIndex>processCommand || processIndex<1){
        printf("Invalid process value\n");
        return;
    }
    if(getpgid(processesPID[processIndex-1])>=0){
        kill(processesPID[processIndex-1], signalValue);
        return;
    }else{
        printf("Process terminated already\n");
        return;
    }
}