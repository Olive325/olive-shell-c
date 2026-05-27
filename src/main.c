#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*CommandFunc)(char *);

typedef struct{
  char *command_name;
  CommandFunc cmdPtr;
}BuiltinCommands;

void command_exit(char *args);
void command_echo(char *args);
void command_type(char *args);

const BuiltinCommands builtins[] = {
  {"exit", &command_exit},
  {"echo", &command_echo},
  {"type", &command_type}
};

#define BUILTINS_LEN 3

int main(int argc, char *argv[]) {

  // Flush after every printf  
  setbuf(stdout, NULL);
  
  while(1){

    printf("$ ");

    char input[1024];

  	fgets(input, sizeof(input), stdin);
  	input[strcspn(input, "\n")] = '\0';
  	
  	char *command = strtok(input, " ");
  	char *args = strtok(NULL, "");
  	
  	if (args == NULL){
      args = "";
	}
	
	int found = 0;

    for (int i = 0; i < BUILTINS_LEN; i++){
  	
  	  if (strcmp(builtins[i].command_name, command) == 0){
  		  
  		  builtins[i].cmdPtr(args);
  		  found = 1;
  		  break;
	    }
    }
    if (!found){
      printf("%s: command not found\n", command);
	}
  };
  return 0; 
}

void command_exit(char *args){
  exit(0);
}

void command_echo(char *args){
  printf("%s\n", args);
}

void command_type(char *args){
	
  int found = 0;
  
  for (int i = 0; i < BUILTINS_LEN; i++){
	if (strcmp(builtins[i].command_name, args) == 0){
		printf("%s is a shell builtin\n", args);
		found = 1;
		break;
	}
  }
  if (!found){
	printf("%s: not found\n", args);
  }
}