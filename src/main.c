#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

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

int main() {
  // Flush after every printf  
  setbuf(stdout, NULL);
  
  while(1){
    printf("$ ");

    char input[1024];

  	fgets(input, sizeof(input), stdin);
  	input[strcspn(input, "\n")] = '\0';
  	
  	char *command = strtok(input, " ");
  	char *args = strtok(NULL, "");
  	
  	int found = 0;
  	
  	if (args == NULL){
      args = "";
	  }
	  if (!command){
      continue;
	  }

    for (int i = 0; i < BUILTINS_LEN; i++){
  	
  	  if (strcmp(builtins[i].command_name, command) == 0){
  		  
  		  builtins[i].cmdPtr(args);
  		  found = 1;
  		  break;
	    }
    }

    if (found){
      continue;
    }

    char *env_path = getenv("PATH");

    if (env_path == NULL){
      printf("PATH not set\n");
    }

    char *path = strdup(env_path);

    for (char *dir = strtok(path,":"); dir != NULL; dir = strtok(NULL,":")){

      char fpath[1024];
      snprintf(fpath, sizeof(fpath), "%s/%s", dir, command);

      if (access(fpath, X_OK) == 0){
        pid_t process = fork();
        if (process < 0){
          perror("fork fail");
          exit(1);
        }
        char *args1 = strtok(args, " ");
        char *args2 = strtok(NULL, "");
        if (args1 == NULL){
          args1 = "";
	      }
        if(args2 == NULL){
          args2 = "";
        }
        execl(fpath, command, args1, args2, (char*)NULL);
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
	    return;
	  }
  }
  char *env_path = getenv("PATH");
  if (env_path == NULL){
    printf("PATH not set\n");
    return;
  }

  char *path = strdup(env_path);

  
  for (char *dir = strtok(path,";"); dir != NULL; dir = strtok(NULL, ";")){

  	char fpath[1024];
  	snprintf(fpath, sizeof(fpath), "%s\\%s", dir, args);
  	
	  if (access(fpath, X_OK) == 0){
	    printf("%s is %s\n", args, fpath);
	    found = 1;
	    free(path);
	    return;
	  }
  }
  free(path);
  
  
  if (!found){
	  printf("%s: not found\n", args);
  }
}