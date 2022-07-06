#ifndef PARSER_H
#define PARSER_H

#define MAX_STR_LEN 4096
#define MAX_PATH_LEN 1024

#define CASE_SENSITIVE 0
#define CASE_INSENSITIVE 1
#define ONLY_START 1
#define ONLY_END 1

struct Strings
{
    // Base Str
    char baseStrings[100][MAX_STR_LEN];
    int   numOfBaseStr;
    // Target Str
    char targetString[MAX_STR_LEN];
    // Parser features
    int caseSensitivity;
    int onlyStart;
    int onlyEnd;
    int repeatLimit;
};
struct Strings getArgStructure(char rawStr[], char * fileContext);

int   changeOccurrence    (char* str, char * delimiter, struct Strings * argStruct);
void  changeAllOccurences (char * argumentStr, char * fileContext);
char* myStrlwr            (char * str);
char* myStristr           (char *__haystack, char *__needle);
void  getPart1            (char * str, char * part1, char * delimiter, char * firstOccurence);
void  getPart2            (char * str, char * part2, char * delimiter, char * firstOccurence);


#endif