#include"headers.h"
#include"global.h"

void initialisePrompt(){
    char sysname[VARIABLE_LENGTH];
    if(gethostname(sysname, sizeof(sysname)) == 1){
        perror("Error in getting the host name: ");
        exit(1);
    }
    
    if(getlogin() == NULL){
        perror("Error in getting the user name: ");
        exit(1);
    }
    prompt[0] = '\0';
    strcat(prompt, "<");
    strcat(prompt, getlogin());
    strcat(prompt, "@");
    strcat(prompt, sysname);
    strcat(prompt, ":");
    strcat(prompt, pathOfShell);
    strcat(prompt, ">");
}

