#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  while(1){
  // Flush after every printf
  setbuf(stdout, NULL);

  // TODO: Uncomment the code below to pass the first stage
  printf("$ ");

  char command[1024];

  fgets(command, sizeof(command), stdin);

  command[strcspn(command, "\n")] = '\0';

  if (strcmp(command, "exit") == 0){
    break;
  }
  if (strncmp(command, "echo ", 5) == 0){
    printf("%s\n", command + 5);
    continue;
  }

  printf("%s: command not found\n", command);


  };
  

  return 0;
}
