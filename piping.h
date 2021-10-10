#include"global.h"
#include"headers.h"
#include"input.h"
void doPiping(int in, int out, char* command, int* flag)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        argumentInput[argumentNumber] = NULL;
        if (in != 0)
        {
            dup2(in, 0);
            close(in);
        }
        if (out != 1)
        {
            dup2(out, 1);
            close(out);
        }
        checkCommand(command, flag);
        exit(0);
    }
    waitpid(pid, NULL, WUNTRACED);
}
void checkPipes(char* command, int* flag){
    int fd[2];
    int input = 0;
    int tempArgumentNumber = argumentNumber;
    char** tempArgumentInput = argumentInput;
    argumentNumber = 0;
    for(int i = 0 ; i < argumentNumber ; i++){
        if(argumentInput[i] == "|"){
            pipe(fd);
            doPiping(input, fd[1], command, flag);
            close(fd[1]);
            input = fd[0];
            argumentInput = argumentInput + argumentNumber;
            argumentInput++;
            argumentNumber = 1;
            i++;
        }else{
            argumentNumber++;
        }
    }
    doPiping(input, 1, command, flag);
    argumentNumber = tempArgumentNumber;
    argumentInput = tempArgumentInput;
}