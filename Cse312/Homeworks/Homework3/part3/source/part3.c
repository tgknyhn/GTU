#include "../header/part3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

char operation[MAX_OPERATION_LEN];

int main(int argc, char *argv[])
{
    // Controlling command line arguments
    ctrcla(argc, argv);
    // execute the operation
    executeOperation();
    
    return 0;
}


void ctrcla(int argc, char *argv[]) {
    if(argc != 3 && argc != 4)
        exitWithError("Incorrect command line argument count.\n");
    
    strcpy(operation, argv[2]);   
}

void executeOperation() {
    exitWithError("Operation is not implemented.\n");
}

void exitWithError(char * errorMessage) {
    perror(errorMessage);
    exit(EXIT_FAILURE);
}


