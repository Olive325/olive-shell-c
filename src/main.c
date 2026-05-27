#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
  char *command_name;
  void (*cmdPtr)();
}BuiltinCommands;

void command_exit();
void command_echo(void *);
void command_type(void *);

const BuiltinCommands builtins[] = {
  {"exit", &command_exit},
  {"echo ", &command_echo},
  {"type ", &command_type}
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

    for (int i = 0; i <= BUILTINS_LEN; i++){
  	
  	  int cmd_name_len = sizeof(builtins[i].command_name) - 3;
  	  
  	  if (i == BUILTINS_LEN){
		
        printf("%s: command not found\n", input);
	    break;
	  }
  	
  	  if (strncmp(builtins[i].command_name, input, cmd_name_len) == 0){
  		  
  		  builtins[i].cmdPtr(&input[cmd_name_len]);
  		
  		  break;
	    }
    }

  };
  return 0; 
}

void command_exit(){
  exit(0);
}

void command_echo(void *input_address_){
  int input_address = *(int*)input_address_;
  printf("%s\n", input_address_);
}

void command_type(void *input_address_){
  
  char *input_address = malloc(sizeof((char*)input_address_));
  
  if (input_address == NULL){
  	printf("memory allocation failed");
  	exit(1);
  }
  
  strcpy(input_address,(char*)input_address_);
  int input_len = sizeof(input_address);
  
  for (int i = 0; i <= BUILTINS_LEN; i++){
  	
  	char *current_command = malloc(sizeof(builtins[i].command_name));
  	
  	if (current_command == NULL){
  		printf("memory allocation failed");
  		exit(1);
	  }
	  
	  strcpy(current_command, builtins[i].command_name);
	  current_command[strcspn(current_command, " ")] = '\0';
  	
  	if (i == BUILTINS_LEN){
  		printf("%s: not found\n", input_address);
  		free(input_address);
  		break;
	  }
	
	  if (strncmp(current_command, input_address, input_len) == 0){
		  printf("%s is a shell builtin\n", input_address);
		  free(input_address);
		  break;
	  }
  }
}
