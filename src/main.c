#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef void (*CommandFunc)(char **);

typedef struct{
  char *command_name;
  CommandFunc cmdPtr;
}BuiltinCommands;

void command_exit(char **args);
void command_echo(char **args);
void command_type(char **args);

const BuiltinCommands builtins[] = {
  {"exit", &command_exit},
  {"echo", &command_echo},
  {"type", &command_type}
};

#define BUILTINS_LEN (sizeof(builtins) / sizeof(builtins[0]))

int main(int argc, char *argv[]) {
  
  
  // Flush after every printf
  setbuf(stdout, NULL);

  while(1){
    printf("$ ");

    char input[1024];
    int found = 0;

  	fgets(input, sizeof(input), stdin);
  	input[strcspn(input, "\n")] = '\0';
  	
  	char **args = NULL;
  	int arg_count = 0;
  	char *token = strtok(input, " ");
  	
  	while (token != NULL){
  		
  	  char **temp = realloc(args, sizeof(char *) * (arg_count + 1));
  	  
  	  if (temp == NULL){
  	  	free(args);
  	    perror("realloc");
		exit(1);
	  }
	  
	  args = temp;
	  
	  args[arg_count++] = token;
	  
	  token = strtok(NULL, " ");
	}
	
	char **temp = realloc(args, sizeof(char *) * (arg_count + 1));
	
	if(temp != NULL){
	  args = temp;
	  args[arg_count] = NULL;
	}
	
	if (arg_count == 0){
	  free(args);
      continue;
	}

    for (int i = 0; i < BUILTINS_LEN; i++){

  	  if (strcmp(builtins[i].command_name, args[0]) == 0){

  		  builtins[i].cmdPtr(args);
  		  found = 1;
  		  break;
	    }
    }

    if (found){
      free(args);
      continue;
    }

    
	pid_t process_id = fork();
	if (process_id < 0){
	  perror("fork");
	  exit(1);
	}
	else if(process_id == 0){
	
	  execvp(args[0], args);
	  exit(127);
	
	  perror("execvp");
	  exit(1);
	}
	else{
	  int status;
   	  waitpid(process_id, &status, 0);
   	  if (WIFEXITED(status) && WEXITSTATUS(status) == 127){
      printf("%s: command not found\n", args[0]);
  	}
	}
    
  	free(args);
  };
  return 0; 
}

void command_exit(char **args){
  exit(0);
}

void command_echo(char **args){
  for (int i = 1; args[i] != NULL; i++){
  	printf("%s", args[i]);
  	if (args[i+1] != NULL){
  		printf(" ");
	}
  }
  printf("\n");
}

void command_type(char **args){
  if (args[1] == NULL){
  	printf("type: missing operand\n");
  	return;
  }

  int found = 0;

  for (int i = 0; i < BUILTINS_LEN; i++){
	  if (strcmp(builtins[i].command_name, args[1]) == 0){
	    printf("%s is a shell builtin\n", args[1]);
	    found = 1;
	    return;
	  }
  }
  char *env_path = getenv("PATH");
  if (env_path == NULL){
    printf("PATH not set\n");
    return;
  }

  char *path = strdup(env_path);


  for (char *dir = strtok(path,":"); dir != NULL; dir = strtok(NULL, ":")){

  	char fpath[1024];
  	snprintf(fpath, sizeof(fpath), "%s/%s", dir, args[1]);

	  if (access(fpath, X_OK) == 0){
	    printf("%s is %s\n", args[1], fpath);
	    found = 1;
      	free(path);
	    return;
	  }
  }
  free(path);


  if (!found){
	  printf("%s: not found\n", args[1]);
  }
}
