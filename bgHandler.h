#include"headers.h"
#include"global.h"

void bg(){
    int jobNumber;
    if(argumentNumber != 2 ){
        printf("%s: Invalid number of arguments\n", argumentInput[0]);
        return;
    }
    jobNumber = atoi(argumentInput[1]);
    if(jobNumber > processCommand || jobNumber < 1){
        printf("%s: Invalid job number\n", argumentInput[0]);
        return;
    }
    if(getpgid(processesPID[jobNumber-1])>0){
        kill(processesPID[jobNumber-1], SIGCONT);
    }else{
        printf("Process already terminated.\n");
    }
}