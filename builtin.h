#include"global.h"
#include"headers.h"


void echo(){
    for(int i = 1; i < argumentNumber; i++){
        printf("%s ", argumentInput[i]);
    }
    printf("\n");
    fflush(stdout);
}

void checkLeadingSlash(char* string){
    if(string[strlen(string)-1] != '/')
        strcat(string, "/");
    return;
}

void getPathFromShell(char* path){
    // char **sepratedFolders = (char **)malloc(MAX_PATH_DEPTH*sizeof(char *));
    // int k = 0;
    // char* tokenized;
    // tokenized = strtok(path, "/");
    // while(tokenized!=NULL){
    //     sepratedFolders[k++] = tokenized;
    //     tokenized = strtok(NULL, "/");  
    // }
    // sepratedFolders[k] = tokenized;
    int i, j;
    for(i = 0 ; i < strlen(originalPath) ; i++){
        if(path[i] != originalPath[i]){
            strcpy(pathOfShell, path);
            return;
        }
    }
    pathOfShell[0] = '~';
    for(j = i; j<strlen(path) ;j++){
        pathOfShell[j-i+1] = path[j];
    }
    pathOfShell[j-i+1] = '\0';
    return;
}

void cd(){
    char pwdPath[VARIABLE_LENGTH] = "";
    char locationPath[VARIABLE_LENGTH] = "";
    char pathWithoutRoot[VARIABLE_LENGTH] = "";
    if(argumentNumber>2){
        printf("Too many arguments for cd command\n");
        return;
    }
    if(argumentNumber == 1 || strcmp(argumentInput[1], "~") == 0){
        DIR *dir = opendir(originalPath);
        if(dir == NULL){
            perror("Path not available");
            return;
        }
        strcpy(previousPath, pathOfShell);
        strcpy(pathOfShell, "~");
        chdir(originalPath);
        return;
        // get to original path
    }
    else if(argumentInput[1][0] == '/'){
        DIR *dir = opendir(argumentInput[1]);
        if(dir == NULL){
            perror("Path not available");
            return;
        }
        strcpy(previousPath, pathOfShell);
        chdir(argumentInput[1]);
        getcwd(pwdPath, sizeof(pwdPath));
        getPathFromShell(pwdPath);
        return;
        // relocating to exact path
    }
    else if(strcmp(argumentInput[1], ".") == 0){
        return;
        // relocate to the same directory
    }
    else if(strcmp(argumentInput[1], "-") == 0){
        strcpy(argumentInput[1], previousPath);
        cd();
        //move to previous path directory
    }
    else{
        // Normal execution of cd
        if(pathOfShell[0]=='/'){
            strcat(locationPath, pathOfShell);
            // if(locationPath[strlen(locationPath)-1] != '/')
            //     strcat(locationPath, "/");
            checkLeadingSlash(locationPath);
            strcat(locationPath, argumentInput[1]);
            // printf("%s\n", locationPath);
            DIR *dir = opendir(locationPath);
            if(dir == NULL){
                perror("Path not available");
                return;
            }
            strcpy(previousPath, pathOfShell);
            chdir(locationPath);
            getcwd(pwdPath, sizeof(pwdPath));
            getPathFromShell(pwdPath);
            return;
        }
        else if(pathOfShell[0]=='~'){
            strcat(locationPath, originalPath);
            char* tokenized;
            tokenized = strtok(pathOfShell, "~");
            if(tokenized != NULL){
                // if(locationPath[strlen(locationPath)-1] != '/')
                //     strcat(locationPath, "/");
                checkLeadingSlash(locationPath);
                strcat(locationPath, tokenized);
            }
            // if(locationPath[strlen(locationPath)-1] != '/')
            //     strcat(locationPath, "/");
            checkLeadingSlash(locationPath);
            strcat(locationPath, argumentInput[1]);
            DIR *dir = opendir(locationPath);
            if(dir == NULL){
                perror("Path not available");
                return;
            }
            strcpy(previousPath, pathOfShell);
            chdir(locationPath);
            getcwd(pwdPath, sizeof(pwdPath));
            getPathFromShell(pwdPath);
            return;
        }
    }
}

void pwd(){
    char pwdPath[VARIABLE_LENGTH];
    getcwd(pwdPath, sizeof(pwdPath));
    printf("%s\n", pwdPath);
}