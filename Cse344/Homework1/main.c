// Basic Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// My Libraries
#include "file.h"
#include "parser.h"

void controlArguments(int argc, char * argv[]) {
    char errorMsg[MAX_STR_LEN];
    
    if(argc != 3) {
        strcpy(errorMsg, "Error: You need to enter 3 arguments.(1: Execution | 2: Replacement strings | 3: InputFilePath)\n");
        write(2, errorMsg, strlen(errorMsg));
        exit(0);
    }

    char check1 = argv[1][0];
    char check2 = argv[1][strlen(argv[1])-1];

    if(check1 != '/' || !(check2 == '/' || check2 == 'i')) {
        strcpy(errorMsg, "Error: Wrong argument syntax. (Ex: ./hw1 '/str1/str2/' \"file.txt\")\n");
        write(2, errorMsg, strlen(errorMsg));
        exit(0);
    }
}

int main(int argc, char * argv[]) {
    /* INITIALIZING */
    controlArguments(argc, argv);
    // Getting str1 and str2 from command line arguments
    char argumentStr[MAX_STR_LEN]; strcpy(argumentStr, argv[1]);       
    // Initializing File Path
    char inputFilePath[MAX_PATH_LEN]; strcpy(inputFilePath, argv[2]);
    // Getting length of the file
    int  fileLength = getFileLength(inputFilePath); 
    // Allocating enough space for fileContext(gave extra space because of replacement)
    char fileContext[MAX_STR_LEN];

    if(fileLength > MAX_STR_LEN) {
        char errorMsg[MAX_STR_LEN];
        strcpy(errorMsg, "Error: .txt file is too big to read (max 2048 characters)\n");
        write(2, errorMsg, strlen(errorMsg));
        exit(0);
    }

    // Reading context from the file
    readFromFile(inputFilePath, fileContext, fileLength);
    // Changing occurrences
    changeAllOccurences(argumentStr, fileContext);
    // Writing new context to the file
    writeToFile(inputFilePath, fileContext);

    return 0;
}


/* MakeFile commands */

//runWithArgument:
//	gcc main.c parser.c file.c -o hw1 && ./hw1 '/Window[sz]*/Linux/i;/close[dD]/open/i' "target.txt"

//deleteTxt:
//	rm "target.txt"

//runWithWrongArgument:
//	gcc main.c parser.c file.c -o hw1 && ./hw1 '/^Window[sz]*/Linux/i;/close[dD]!/open' "target123.txt"
