#include"global.h"
#include"headers.h"

int checkValid(){
	struct stat sb;
	char* pathAtProc = (char *)malloc(VARIABLE_LENGTH * sizeof(char));
	strcat(pathAtProc, "/proc/");
	strcat(pathAtProc, argumentInput[1]);
	if(stat(pathAtProc, &sb) < 0 && errno == ENOENT){
		return 0;
	}
	else{
		return 1;
	}

}

int checkHome(char* path){
	int len = strlen(originalPath);
	int len2 = strlen(path);
	if(len2 > len){
		for(int i = 0; i < len ; i++){
			if(originalPath[i] != path[i]){
				return 0;
			}
		}
		return 1;
	}
	return 0;

}

void pinfo(){
	char* pathVariable = (char *)malloc(VARIABLE_LENGTH * sizeof(char));
	char* tempPathVariable = (char *)malloc(VARIABLE_LENGTH * sizeof(char));
	strcat(pathVariable, "/proc/");
	char getString[20];
	if(argumentNumber == 1){
		// print the PID number
		printf("pid -- %d\n", shellPID);
		sprintf(getString, "%d", shellPID);
		strcat(pathVariable, getString);
	}
	else if(argumentNumber == 2){
		if(checkValid()){
			printf("pid -- %s\n",argumentInput[1]);
			strcat(pathVariable, argumentInput[1]);
		}	
		else{
			printf("PID does not exist \n");
			return;
		}
		// check if the PID is valid
	}
	else{
		printf("Too many arguments for pinfo command");
		return;
		// invalid number of arguments for pinfo argument
	}
	char** sepratedValues = (char **)malloc(VARIABLE_LENGTH*sizeof(char *));
	char line[VARIABLE_LENGTH];
	char newLine[VARIABLE_LENGTH];
	strcpy(tempPathVariable, pathVariable);	
	int k = 0;
	char* tokenized;
	strcat(pathVariable, "/stat");


	FILE *reader = fopen(pathVariable, "r");
	fgets(line, VARIABLE_LENGTH, reader);

	tokenized = strtok(line, " ");
		while(tokenized!=NULL){
		sepratedValues[k] = tokenized;
		tokenized = strtok(NULL, " ");
		k++;
	}
	
	printf("Process Status -- ");
	if((sepratedValues[2][0] == 'S' || sepratedValues[2][0] == 'R')&& (sepratedValues[4]==sepratedValues[7])){
		printf("%s+\n",sepratedValues[2]);
	}
	else{
		printf("%s\n",sepratedValues[2]);
	}
	printf("memory -- ");
	printf("%s\n",sepratedValues[22]);
	strcat(tempPathVariable, "/exe");
	
	readlink(tempPathVariable, newLine, VARIABLE_LENGTH);

	char executablePath[VARIABLE_LENGTH];
	printf("Executable Path -- ");
	if(checkHome(newLine)){
		strcpy(executablePath, "~/");
		int i = 2;
		for(int j = strlen(originalPath)+1; j < strlen(newLine); j++){
			executablePath[i] = newLine[j];
			i++;
		}
		executablePath[i] = '\0';
		printf("%s\n", executablePath);
	}
	else{
		printf("%s\n",newLine);
	}
	return;
}