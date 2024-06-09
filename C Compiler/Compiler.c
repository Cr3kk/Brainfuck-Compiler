//TODO: Add error handling
//TODO: optimize
//TODO: Add comments
//TODO: fix, something went wrong, the loops broke at some point

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

char* ReadFile();
void ExecuteCode(char* code, char* memory, int curPointer);

int main() {
    const char *newDir = "../";
    if (chdir(newDir) != 0) {
        perror("chdir");
        return 1;
    }

    char* code = ReadFile();
    printf("%s\n", code);

    char* codeFree = (char *)malloc(strlen(code) + 1); // Allocate memory for codeFree
    strcpy(codeFree, ""); // Initialize codeFree with an empty string

    for (int i = 0; i < strlen(code); i++) { // Loop through each character in code
        if (code[i] == '+' || code[i] == '-' || code[i] == '>' || code[i] == '<' || code[i] == '.' || code[i] == ',' || code[i] == '[' || code[i] == ']') {
            strncat(codeFree, &code[i], 1); // Concatenate valid characters to codeFree
        }
    }

    char* memory = (char *)malloc(30000 * sizeof(char)); // Allocate memory for memory array
    memset(memory, 0, 30000); // Initialize memory array with zeros
    int curPointer = 0;
    ExecuteCode(codeFree, memory, curPointer);

    printf("\n");
    free(memory);
    free(code);
    free(codeFree); // Free allocated memory for codeFree
    return 0;
}

char* ReadFile() {
    FILE *fptr;
    fptr = fopen("target.bf", "r");
    char *code = (char *)malloc(sizeof(char) * 10000); // Allocate memory for code
    fgets(code, 10000, fptr);
    fclose(fptr);
    return code;
}

void ExecuteCode(char* code, char* memory, int curPointer) {
    int instructionPointer = 0;
    int loopStart = 0;

    while(instructionPointer < strlen(code)) {
        char i = code[instructionPointer];

        switch(i) {
            case '>':
                curPointer++;
                break;
            case '<':
                curPointer--;
                break;
            case '+':
                memory[curPointer]++;
                break;
            case '-':
                memory[curPointer]--;
                break;
            case '.':
                putchar(memory[curPointer]);
                break;
            case ',':
                {
                    char in = getchar();
                    int asciiValue = (int)in;
                    memory[curPointer] = asciiValue;
                    break;
                }
            case '[':
                if(memory[curPointer] == 0) {
                    int bracketCount = 1;
                    while(bracketCount > 0) {
                        instructionPointer++;
                        if(code[instructionPointer] == '[') {
                            bracketCount++;
                        } else if(code[instructionPointer] == ']') {
                            bracketCount--;
                        }
                    }
                }
                else {
                    loopStart = instructionPointer;
                }
                break;
            case ']':
                if(memory[curPointer] != 0) {
                    int bracketCount = 1;
                    while(bracketCount > 0) {
                        instructionPointer--;
                        if(code[instructionPointer] == '[') {
                            bracketCount--;
                        } else if(code[instructionPointer] == ']') {
                            bracketCount++;
                        }
                    }
                }
                break;
            default:
                break;
        }
        instructionPointer++;
    }
}
