#ifndef LS
#define LS

#include"global.h"
#include"headers.h"


void printFullInfo(char* path){
    struct stat sb;
    char* permission = (char *)malloc(10*sizeof(char));
    for(int i = 0; i < 10; i++){
        permission[i]='-';
    }
    stat(path, &sb);
    if(S_ISDIR(sb.st_mode))
        permission[0] = 'd';
    if(sb.st_mode & S_IRUSR)
        permission[1] = 'r';
    if(sb.st_mode & S_IWUSR)
        permission[2] = 'w';
    if(sb.st_mode & S_IXUSR)
        permission[3] = 'x';
    if(sb.st_mode & S_IRGRP)
        permission[4] = 'r';
    if(sb.st_mode & S_IWGRP)
        permission[5] = 'w';
    if(sb.st_mode & S_IXGRP)
        permission[6] = 'x';
    if(sb.st_mode & S_IROTH)
        permission[7] = 'r';
    if(sb.st_mode & S_IWOTH)
        permission[8] = 'w';
    if(sb.st_mode & S_IXOTH)
        permission[9] = 'x';
    
    printf("%s ", permission);
    printf("%2hu ", sb.st_nlink);
    struct passwd *user = getpwuid(sb.st_uid);
    printf("%s ", user->pw_name);
    struct group *grpid = getgrgid(sb.st_gid);
    printf("%s ", grpid->gr_name);
    printf("%9lld ", sb.st_size);
    // STANDARD TIME CALCULATION, SOURCE STACKOVERFLOW
    time_t *t = malloc(1024);
    *t = sb.st_mtime;
    struct tm time = *localtime(t);
    char *month = mon[time.tm_mon];
    printf("%s ", month);
    printf("%2d ", time.tm_mday);
    printf("%02d:", time.tm_hour);
    printf("%02d ", time.tm_min);
}

void ls(){
    // getting the information about the flag   
    // implementing A and L flags only
    struct stat sb;
    int aFlag = 0;
    int lFlag = 0;
    int numFolders = 0;
    char folders[MAX_ARGUMENTS][VARIABLE_LENGTH];
    char* pathFolder = (char *)malloc(VARIABLE_LENGTH*sizeof(char));
    char* fileName = (char *)malloc(VARIABLE_LENGTH*sizeof(char));
    char* temp = (char *)malloc(VARIABLE_LENGTH*sizeof(char));
    for(int i = 1; i < argumentNumber; i++){
        if(argumentInput[i][0] == '-'){
            if(strlen(argumentInput[i])==2){
                if(argumentInput[i][1] == 'l')
                    lFlag = 1;
                else if(argumentInput[i][1] == 'a')
                    aFlag = 1;
                else
                    printf("Invalid flag for ls %c\n", argumentInput[i][1]);
            }
            else if(strlen(argumentInput[i])==3){
                if((argumentInput[i][1] == 'l' || argumentInput[i][2] == 'l') && (argumentInput[i][1]=='a' || argumentInput[i][2]=='a')){
                    lFlag = 1;
                    aFlag = 1;
                }
                else{
                    printf("Invalid flag for ls\n");
                }
            }
        }
        else{
            strcpy(folders[numFolders], argumentInput[i]);
            numFolders++;
        }
    }
    if(numFolders == 0){
        strcpy(folders[0], ".");
        numFolders++;
    }
    for(int i = 0; i < numFolders ; i++){
        char* tokenized;
        int total = 0;
        if(folders[i][0] == '/'){
            strcpy(pathFolder, folders[i]);
        }
        else if(folders[i][0] == '~'){
            strcpy(pathFolder, originalPath);
            tokenized = strtok(folders[i], "~");
            if(tokenized!=NULL){
                checkLeadingSlash(pathFolder);
                // if(pathFolder[strlen(pathFolder)-1] != '/')
                //     strcat(pathFolder, "/");
                strcat(pathFolder, tokenized);
            }
        }
        else{
            if(pathOfShell[0] == '~'){
                strcpy(pathFolder, originalPath);
                // if(pathFolder[strlen(pathFolder)-1] != '/')
                //     strcat(pathFolder, "/");
                checkLeadingSlash(pathFolder);
                tokenized = strtok(pathOfShell, "~");
                if(tokenized!=NULL){
                    // if(pathFolder[strlen(pathFolder)-1] != '/')
                    //     strcat(pathFolder, "/");
                    checkLeadingSlash(pathFolder);
                    strcat(pathFolder, tokenized);
                }
                // if(pathFolder[strlen(pathFolder)-1] != '/')
                //     strcat(pathFolder, "/");
                checkLeadingSlash(pathFolder);
                strcat(pathFolder, folders[i]);
            }
            else{
                strcpy(pathFolder, pathOfShell);
                checkLeadingSlash(pathFolder);
                // if(pathFolder[strlen(pathFolder)-1] != '/')
                //     strcat(pathFolder, "/");
                strcat(pathFolder, folders[i]);
            }
            // strcpy(pathFolder, originalPath);
            // tokenized = strtok(pathOfShell, "~");
            // if(tokenized!=NULL){
            //     if(pathFolder[strlen(pathFolder)-1] != '/')
            //         strcat(pathFolder, "/");
            //     strcat(pathFolder, tokenized);
            // }
            // if(pathFolder[strlen(pathFolder)-1] != '/')
            //     strcat(pathFolder, "/");
            // strcat(pathFolder, folders[i]);
        }
        if(stat(pathFolder, &sb) == 0 && S_ISDIR(sb.st_mode)){
            if(numFolders > 1){
                printf("%s/:\n", folders[i]);
            }
            DIR *dr = opendir(pathFolder);
            if(dr == NULL){
                perror("cd: ");
            }
            struct dirent *de, *de2;
            if(lFlag){
                while ((de = readdir(dr)) != NULL){
                    if((de->d_name[0]!='.') || (de->d_name[0]=='.' && aFlag)){
                        strcpy(fileName, pathFolder);
                        if(fileName[strlen(fileName)-1] != '/')
                            strcat(fileName, "/");
                        strcat(fileName, de->d_name);
                        stat(fileName, &sb);
                        total += sb.st_blocks;
                    }
                }
                printf("total: %d\n",total);
                rewinddir(dr);
                // get the total block size
            }
            while ((de2 = readdir(dr)) != NULL){
                if(lFlag){
                    if((de2->d_name[0]!='.') || (de2->d_name[0]=='.' && aFlag)){
                        strcpy(fileName, pathFolder);
                        // if(fileName[strlen(fileName)-1] != '/')
                        //     strcat(fileName, "/");
                        checkLeadingSlash(fileName);
                        strcat(fileName, de2->d_name);
                        printFullInfo(fileName);
                        printf("%s\n", de2->d_name);
                    }
                }
                else{
                    if((de2->d_name[0]!='.') || (de2->d_name[0]=='.' && aFlag)){
                        printf("%s\n", de2->d_name);
                    }
                }
            }
            closedir(dr);
        }
        else{
            if(lFlag){
                printf("\n");
                printFullInfo(pathFolder);
                printf("%s\n", folders[i]);
            }
            else{
                printf("\n%s\n", folders[i]);
            }
            // diplay the file
        }
    }
}

#endif