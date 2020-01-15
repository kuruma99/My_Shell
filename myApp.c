#include<stdio.h>
#include<stdlib.h>

#define LINE_SIZE 1024
#define TOKEN_SIZE 64
#define TOKEN_DELIM " \n"

char* readLine(void);
//char** parse(char* line);


int main(){
    char *input = readLine();
    printf("You just printed %s", input);

    // char* line = "My name is Mridul";
    // char** tokens = parse(line);
    
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

// char** parse(char* line){
//     int bufSize = TOKEN_SIZE;
//     int position = 0;
//     char** tokens = malloc(sizeof(char*) * bufSize);
//     char* token;

//     token = strok(line, TOKEN_DELIM);
//     while(token != NULL){
//         tokens[position] = token;
//         position++;
//         if(position >= bufSize){
//             bufSize += TOKEN_SIZE;
//             tokens = realloc(tokens, bufSize * sizeof(char*));
//         }
//         token = strtok(NULL, TOKEN_DELIM);
//     }
//     tokens[position] = NULL;
//     return tokens;
// }


