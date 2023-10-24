#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>

void parseCommand(char* input, char* tokens[]);
void executeBuildIn(char* tokens[]);
void executeCommand(char* tokens[]);

int main(){


	//char buffer[1024];
	//char *inputCommand = buffer;
	//size_t bufsize = 1000;
	size_t command;

	char* inputCommand = NULL;
	size_t inputSize =1000;
	char* tokens[10];

	while(1){
		printf("fsh> ");
		command = getline(&inputCommand, &inputSize, stdin);


		if(command == -1){
			perror("INPUT ERROR");
			break;
		}

		inputCommand[strcspn(inputCommand,"\n")] ='\0';
		parseCommand(inputCommand, tokens);
		//printf("CHECK 2: IN MAIN AFTER PARSE:\n");

		if(tokens[0]!=NULL){
			if(strcmp(tokens[0], "exit") ==0 || strcmp(tokens[0], "cd") == 0){
				executeBuildIn(tokens);
			}else{
				executeCommand(tokens);
		}
	
		}
	}

	free(inputCommand);
	return 0;

}

void parseCommand(char* input, char* tokens[]){
	char* token = strtok(input, " ");
	int tokenCount =0;

	while (token != NULL){
		tokens[tokenCount] = token;
		tokenCount++;
		token = strtok(NULL, " ");
	}

	tokens[tokenCount] = NULL;
}

void executeBuildIn(char* tokens[]){
	if(strcmp(tokens[0], "exit") == 0){
		exit(0);
	}
	else if(strcmp(tokens[0], "cd") == 0){
		if(tokens[1] == NULL){
			fprintf(stderr, "cd missing argument\n");
		}else if (tokens[2] != NULL){
			fprintf(stderr,"cd: To many arguments\n");
		} else{
			if(chdir(tokens[1]) == -1){
				perror("chdir error");
			}
			//char cwd[1024];
			//if(getcwd(cwd, sizeof(cwd)) !=NULL){
				//printf("%s\n",cwd);
			//}else{
			//	perror("getcwd error");
			
		}
		
	}

}

void executeCommand(char* tokens[]){
	pid_t childPID;
	int status;

	childPID = fork();
	if(childPID == 0){
		if(execvp(tokens[0], tokens) == -1){
			perror("execvp error");
			exit(1);
		}
	}else if (childPID >0){
		waitpid(childPID, &status, 0);
	}else{
		perror("fork error");
	}
}
