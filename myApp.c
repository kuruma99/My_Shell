#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LINE_SIZE 1024
#define TOKEN_SIZE 64
#define TOKEN_DELIM " \n\t"

char* readLine(void);
char** parseLine(char* line);
int launch(char **args);
int msh_cd(char **args);
int msh_help(char **args);
int msh_exit(char **args);
int msh_execute(char **args);
void loop(void);
int msh_pwd(char **args);
int msh_mkdir(char **args);


int main(){
    loop();
    return 0;
    
}

void loop(void){ 
  char *line;
  char **args;
  int status;

  do {
    printf("\nshell>");
    line = readLine();
    args = parseLine(line);
    status = msh_execute(args);

    free(line);
    free(args);
  } while (status);
}

char* readLine(void){
        int bufsize = LINE_SIZE;
        int position = 0;
        char *buffer = malloc(sizeof(char) * bufsize);
        int c;

        while(1){
                //Read a character
                c = getchar();

                //If we hit EOF, replace it with a null character and return
                if(c == EOF || c == '\n'){
                        buffer[position] = '\0';
                        return buffer;
                }
                else{
                        buffer[position] = c;
                }
                position++;

                // If we have exceeded the buffer, reallocate.
                if (position >= bufsize) {
                    bufsize += LINE_SIZE;
                    buffer = realloc(buffer, bufsize);
                }

        }
}

char** parseLine(char* line){
    int bufSize = TOKEN_SIZE;
    int position = 0;
    char** tokens = malloc(sizeof(char*) * bufSize);
    char* token;

    token = strtok(line, TOKEN_DELIM);
    while(token != NULL){
        tokens[position] = token;
        position++;
        if(position >= bufSize){
            bufSize += TOKEN_SIZE;
            tokens = realloc(tokens, bufSize * sizeof(char*));
        }
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int launch(char **args){
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "pwd",
  "mkdir"
};

int (*builtin_func[]) (char **) = {
  &msh_cd,
  &msh_help,
  &msh_exit,
  &msh_pwd,
  &msh_mkdir
};

int builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/
int msh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "msh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("msh");
    }
  }
  return 1;
}

int msh_help(char **args)
{
  int i;
  printf("Mridul Singh's Shell\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int msh_exit(char **args)
{
  return 0;
}

int msh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return launch(args);
}

int msh_pwd(char **args) 
{ 
    char cwd[1024]; 
    getcwd(cwd, sizeof(cwd)); 
    printf("\nDir: %s", cwd); 
    return 1;
} 

int msh_mkdir(char **args) 
{ 
    int result = mkdir(args[1], 0777); 
    if(result == 0)
        printf("\nDirectory Created\n");     
    return 1;
} 


