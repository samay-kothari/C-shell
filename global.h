#ifndef GLOBAL_VARIABLES
#define GLOBAL_VARIABLES

#define VARIABLE_LENGTH 2048
#define MAX_ARGUMENTS 256
#define MAX_COMMANDS 256
#define MAX_PATH_DEPTH 256

int shellPID;
int processesPID[VARIABLE_LENGTH];
int processesIndex[VARIABLE_LENGTH];
char processesNames[VARIABLE_LENGTH][VARIABLE_LENGTH];
char prompt[VARIABLE_LENGTH];
char pathOfShell[VARIABLE_LENGTH] = "~";
char originalPath[VARIABLE_LENGTH];
char previousPath[VARIABLE_LENGTH] = "~";
char mon[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
char **argumentInput;
char **sepratedCommands;
int argumentNumber;
int commandNumber;
int processCommand = 0;
const char *DELIMITER = " \t\r\n\a";
int currentForegroundProcess;

int isStdInChanged = 0;
int isStdOutChanged = 0;
int OriStdOut;
int OriStdIn;

memoryAllocationError(){
	perror("Allocation of memory failed: ");
    exit(1);	
}
void sigStpHandler();
void sigIntHandler(){
    if(currentForegroundProcess != shellPID){
        kill(currentForegroundProcess, SIGINT);
        currentForegroundProcess = shellPID;
    }else{
        printf("\n");
    }
    signal(SIGINT, sigIntHandler);
    signal(SIGTSTP, sigStpHandler);
}

void sigStpHandler(){
    signal(SIGINT, sigIntHandler);
    signal(SIGTSTP, sigStpHandler);
    if(currentForegroundProcess != shellPID){
        kill(currentForegroundProcess, SIGTSTP);
        processesPID[processCommand] = currentForegroundProcess;
        strcpy(processesNames[processCommand], argumentInput[0]);
        processesIndex[processCommand] = processCommand;
        processCommand++;
        printf("Command %s with PID %d has been stopped.\n",argumentInput, currentForegroundProcess);
        currentForegroundProcess = shellPID;
        return;
    }
    else{
        return;
    }
}




#endif