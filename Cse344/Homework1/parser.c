#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

/* Declaring Private parser functions */

char * getFirstOccurrence           (char * str, char * delimiter, int caseSensitivity);
void   controlCaseSensitivity       (struct Strings * argStructure, char * remainingStr);
void   controlSquareBrackets        (struct Strings * argStructure);
void   controlSemiColumn            (char * controlStr, char * fileContext);
char*  controlUpperArrowDollarSign  (char * str1, struct Strings * argStructure);
char*  controlStart                 (char * str, char * firstOccurence, char * delimiter, struct Strings * argStruct, char * p1, char * p2);
char*  controlEnd                   (char * str, char * firstOccurence, char * delimiter, struct Strings * argStruct, char * p1, char * p2);
void   controlStars                 (struct Strings * argStruct, char * str1, int baseIndex);


/* Public Parser Functions */

struct Strings getArgStructure(char rawStr[], char * fileContext) {
    struct Strings argStructure;
    // Initializing some variables of the argStructure
    argStructure.onlyEnd = 0;
    argStructure.onlyStart = 0;
    argStructure.repeatLimit = 20;
    argStructure.numOfBaseStr = 0;
    // Initializing str1, str2 and i
    char * str1 = strtok(rawStr, "/");
    char * str2 = strtok(NULL, "/");
    char * i    = strtok(NULL, "/");


    // Controlling semi columns
    controlSemiColumn(i, fileContext);

    // Controlling Upper arrow and dollar sign
    str1 = controlUpperArrowDollarSign(str1, &argStructure);

    // Extracting Base Strings
    strcpy(argStructure.baseStrings[argStructure.numOfBaseStr++], str1);
    // Extracting Target String
    strcpy(argStructure.targetString, str2);

    // Controlling square brackets
    controlSquareBrackets(&argStructure);
    // Controlling case sensitivity
    controlCaseSensitivity(&argStructure, i);

    // Controlling stars
    int numOfStr = argStructure.numOfBaseStr;
    for(int i=0; i<numOfStr; i++) {
        controlStars(&argStructure, argStructure.baseStrings[i], i);
    }


    //for(int i=0; i<argStructure.numOfBaseStr; i++)
    //    printf("str:%s\n", argStructure.baseStrings[i]);

    return argStructure;
}

void changeAllOccurences(char * argumentStr, char * fileContext) {
    // Extracting str1 and str2 from rawStr
    struct Strings arg = getArgStructure(argumentStr, fileContext);

    // Changing occurences
    for(int i=0; i<arg.numOfBaseStr; i++) {
        int isFinished = 1;

        while(isFinished) {            
            isFinished = changeOccurrence(fileContext, arg.baseStrings[i], &arg);
        }
    }
}

int changeOccurrence(char* str, char * delimiter, struct Strings * argStruct) {
    // Declaring first occurence
    char * firstOccurence = getFirstOccurrence(str, delimiter, argStruct->caseSensitivity);

    // Return 0 if delimiter doesn't exist in the str
    if(firstOccurence == NULL) {
        return 0;
    }

    // Allocating enough space for part1 and part2
    char * part1 = (char*)malloc(MAX_STR_LEN);
    char * part2 = (char*)malloc(MAX_STR_LEN);

    // Getting part1
    getPart1(str, part1, delimiter, firstOccurence);
    // Getting part2
    getPart2(str, part2, delimiter, firstOccurence);


    // Control for ^
    if(argStruct->onlyStart == ONLY_START)
        firstOccurence = controlStart(str, firstOccurence, delimiter, argStruct, part1, part2);
    // Control for $
    if(argStruct->onlyEnd == ONLY_END)
        firstOccurence = controlEnd(str, firstOccurence, delimiter, argStruct, part1, part2);

    // Getting part1 & part2 again if control for start and end executed
    if(argStruct->onlyStart == ONLY_START || argStruct->onlyEnd == ONLY_END) {
        if(firstOccurence == NULL) 
            return 0;
        else {
            part1 = (char*)malloc(MAX_STR_LEN);
            part2 = (char*)malloc(MAX_STR_LEN);
            getPart1(str, part1, delimiter, firstOccurence);
            getPart2(str, part2, delimiter, firstOccurence);
        }
    }

    // Getting parts back together
    strcat(part1, argStruct->targetString);
    strcat(part1, part2);

    // Copying final result to str
    strcpy(str, part1);    

    // Returning positive result
    return 1;
}





/* Private Parser Functions */

char * getFirstOccurrence(char * str, char * delimiter, int caseSensitivity) {
    // Declaring first occurence
    char * firstOccurence;
    // Initializing first occurence
    if(caseSensitivity == CASE_SENSITIVE) 
        firstOccurence = strstr(str, delimiter);
    else 
        firstOccurence = myStristr(str, delimiter);

    return firstOccurence;
}

void getPart1(char * str, char * part1, char * delimiter, char * firstOccurence) {
    int strLength = strlen(str);
    int part1Length = strLength - strlen(firstOccurence);

    for(int i=0; i<part1Length; i++) {
        part1[i] = str[i];
    }
}

void getPart2(char * str, char * part2, char * delimiter, char * firstOccurence) {
    char * tempPart2 = firstOccurence + strlen(delimiter);
    int part2Length = strlen(tempPart2);

    for(int i=0; i<part2Length; i++) {
        part2[i] = tempPart2[i];
    }
}

char* myStrlwr(char * str)
{
  char *tempStr = str;

  while(*tempStr) {
     *tempStr = tolower(*tempStr);
      tempStr++;
  }

  return str;
}

char* myStristr(char *__haystack, char *__needle) {
    // Allocate enough space for temp variables
    char * tempHS = (char*)malloc(strlen(__haystack)*2);
    char * tempN  = (char*)malloc(strlen(__needle)*2);
    
    // Copy context of parameters into temp variables
    strcpy(tempHS, __haystack);
    strcpy(tempN, __needle);

    // Then convert them to lowercase
    myStrlwr(tempHS);
    myStrlwr(tempN);

    // Find address of the first occurence of tempN inside tempHS
    char* firstOccAddress = strstr(tempHS, tempN);

    if(firstOccAddress == NULL)
        return NULL;

    // Find how much address need to be taken to reach first occurence
    int stepsNeeded = strlen(tempHS) - strlen(firstOccAddress);

    // returning __haystack address after iterating stepsneeded times
    return __haystack + stepsNeeded;
}

void controlCaseSensitivity(struct Strings * argStructure, char * remainingStr) {
    if(remainingStr != NULL) {
        if(strcmp(remainingStr, "i") == 0 || strcmp(remainingStr, "i;") == 0) 
            argStructure->caseSensitivity = CASE_INSENSITIVE;
        else
            argStructure->caseSensitivity = CASE_SENSITIVE;
    }
    else
        argStructure->caseSensitivity = CASE_SENSITIVE;

}

void controlSquareBrackets(struct Strings * argStructure) {
    // Flag
    int flag = 0;
    // Getting base
    char * str = argStructure->baseStrings[argStructure->numOfBaseStr-1];
    // To hold letters inside square brackets
    char * letters = (char*)malloc(10);
    // To erase contents of str (because it will include [] in the first address)
    char emptyStr[] = "";

    int strLength = strlen(str);
    int index1 = 0, index2 = 0, letterIndex = 0;
    

    // Extracting []
    for(int i=0; i<strLength; i++) {
        if(str[i] == '[') {
            index1 = i;
            letterIndex = 0;
            while(str[i+1] != ']') {
                letters[letterIndex++] = str[++i];
                if(i == strLength) {
                    break;
                }
                
            }
            flag = 1;
            index2 = i + 1;
        }
    }

    if(flag == 0)
        return;

    char * part1 = (char*)malloc(MAX_STR_LEN);
    char * part2 = (char*)malloc(MAX_STR_LEN);

    // Part1: before [ character
    for(int i=0; i<index1; i++) {
        part1[i] = str[i];
    }

    // Part2: after ] character
    int startIndex = index2 + 1;
    int finishIndex = strlen(str);
    for(int i=startIndex, j=0; i<finishIndex; i++, j++) {
        part2[j] = str[i];
    }


    int letterCount = strlen(letters);
    int strIndex = argStructure->numOfBaseStr - 1;

    // Removing first str
    strcpy(argStructure->baseStrings[strIndex], emptyStr);
    // Assigning new contents
    for(int i=0, j=0; i<letterCount; i++, j++, strIndex++) {
        strcat(argStructure->baseStrings[strIndex], part1);
        strncat(argStructure->baseStrings[strIndex], letters+j, 1);
        strcat(argStructure->baseStrings[strIndex], part2);
    }
    // Assigning number of base strings to structure
    argStructure->numOfBaseStr = strIndex;
    
    free(letters);
}

void controlSemiColumn(char * controlStr, char * fileContext) {
    if(controlStr != NULL) {
        if(strcmp(controlStr, "i;") == 0 || strcmp(controlStr, ";") == 0) {
            // Portition after ;
            char * remainingStr = strtok(NULL, "");
            // Remaningstr doesn't come with / so it is necessary to add it manually
            char * argumentStr = (char*)calloc(strlen(remainingStr)+1, sizeof(char));
            // missing
            char missingChar[1] = "/";
            // Strcat
            strcat(argumentStr, missingChar);
            strcat(argumentStr, remainingStr);
            // Calling change occurence again
            changeAllOccurences(argumentStr, fileContext);
            // Free the argumentStr
            
        }
    }
}

char* controlStart(char * str, char * firstOccurence, char * delimiter, struct Strings * argStruct, char * p1, char * p2) {
    if(argStruct->onlyStart == ONLY_START) {
        int firstCondition, secondCondition;  

        while(firstOccurence != NULL) {
            firstCondition = strlen(p1) == 0;
            secondCondition = *(p1 + (strlen(p1) - 1)) == '\n';

            if(firstCondition || secondCondition) {
                break;
            }
            else {
                firstOccurence = getFirstOccurrence(firstOccurence+1, delimiter, argStruct->caseSensitivity);
                if(firstOccurence == NULL) break;
                p1 = (char*)malloc(MAX_STR_LEN);
                p2 = (char*)malloc(MAX_STR_LEN);
                getPart1(str, p1, delimiter, firstOccurence);
                getPart2(str, p2, delimiter, firstOccurence);
            }
        }        
        if(!firstCondition && !secondCondition) {
            return NULL;
        }
        else {
            return firstOccurence;
        }
    }
    return NULL;
}

char* controlEnd(char * str, char * firstOccurence, char * delimiter, struct Strings * argStruct, char * p1, char * p2) {
    if(argStruct->onlyEnd == ONLY_END) {
        int firstCondition, secondCondition;  

        while(firstOccurence != NULL) {
            firstCondition = strlen(p2) == 0;
            secondCondition = p2[0] == '\n';

            if(firstCondition || secondCondition) {
                break;
            }
            else {
                firstOccurence = getFirstOccurrence(firstOccurence+1, delimiter, argStruct->caseSensitivity);
                if(firstOccurence == NULL) break;
                p1 = (char*)malloc(MAX_STR_LEN);
                p2 = (char*)malloc(MAX_STR_LEN);
                getPart1(str, p1, delimiter, firstOccurence);
                getPart2(str, p2, delimiter, firstOccurence);
            }
        }        
        if(!firstCondition && !secondCondition) {
            return NULL;
        }
        else {
            return firstOccurence;
        }
    }

    return NULL;
}

char* controlUpperArrowDollarSign(char * str1, struct Strings * argStructure) {
    char startStr = str1[0];
    char endStr = str1[strlen(str1)-1];

    if(startStr == '^') {
        argStructure->onlyStart = ONLY_START;
        str1 = str1 + 1;
        
    }
    else if(endStr == '$') {
        char* temp = str1 + strlen(str1) - 1;
        *temp = '\0';
        argStructure->onlyEnd = ONLY_END;
    }
    
    return str1;
}

void controlStars(struct Strings * argStruct, char * str1, int baseIndex) {
    int repeatLimit = argStruct->repeatLimit;
    char starLetter;
    
    char * part1 = (char*)malloc(MAX_STR_LEN);
    char * part2 = (char*)malloc(MAX_STR_LEN);

    // Getting first occurrenct
    char * firstOccurrence = strstr(str1, "*");

    // If there is no *, return
    if(firstOccurrence == NULL)
        return;

    // Then we extract part1 and part2
    getPart1(str1, part1, "*", firstOccurrence);
    getPart2(str1, part2, "*", firstOccurrence);
    
    // Return 0 if part1 has 0 length
    if(strlen(part1) == 0)
        return;

    // Extracting letter that indicated by the star
    starLetter = part1[strlen(part1)-1];
    // Removing that star from the part1
    part1[strlen(part1)-1] = '\0';


    // Creating possible cases
    char * tempStr;
    int index = argStruct->numOfBaseStr;
    
    // First one is used for replacing string with the star
    tempStr = (char*)malloc(MAX_STR_LEN);
    // Adding part1
    strcat(tempStr, part1);
    // Adding letter indicated by the star
    for(int i=0; i<repeatLimit; i++)
        tempStr[strlen(tempStr)] = starLetter;
    // Adding part2
    strcat(tempStr, part2);
    // Assigning new str to strlist
    strcpy(argStruct->baseStrings[baseIndex], tempStr);    
    // Then doing the rest of it
    for(int i=repeatLimit; i>0; i--) {
        tempStr = (char*)malloc(MAX_STR_LEN);
        // Adding part1
        strcat(tempStr, part1);
        // Adding letter indicated by the star
        for(int j=i-1; j>0; j--)
            tempStr[strlen(tempStr)] = starLetter;
        // Adding part2
        strcat(tempStr, part2);
        // Assigning new str to strlist
        strcpy(argStruct->baseStrings[index], tempStr);
        // Incrementing index by 1
        index++;
    }
    argStruct->numOfBaseStr = index;
}
