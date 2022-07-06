/* Libraries */
#include <stdio.h>
#include <signal.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "file.h"
/* Define values */
#define NUM_OF_POINTS 10
#define MAX_CHILD 10000
/* Global variables */
static int sigintFlag = 0;
static int readFD = 0;
static int writeFD = 0;
static int childCount = 0;
static int firstChild = 0;
static int secondChild = 0;
static char * inputFilePath = 0;
static char * outputFilePath = 0;
/* Parent */
void parentProcess(char * fileBuffer, int fileSize);
/* Helper Functions of Parent process */
void  getFilePaths                        (int argc, char *argv[]);
int   getCoordinates                      (char * fileBuffer, int fileIndex, char * x_coordinates, char * y_coordinates, char * z_coordinates);
void  createChildProcess                  (int pidList[MAX_CHILD], char x_coordinates[10], char y_coordinates[10], char z_coordinates[10]);
void  getCovarianceMatricesFromOutputFile (float covarianceMatrices[childCount][3][3]);
float calculateFrobeniusNorm              (float covarianceMatrix[3][3]);
void  getFrobeniusNormOfMatrices          (float covarianceMatrices[childCount][3][3], float frobeniusNorm[childCount]);
float findTwoClosestMatrices              (float frobeniusNorm[childCount]);
/* STDOUT dialogues */
void printFirstDialogue     ();
void printResultToConsole   (float covarianceMatrices[childCount][3][3], float closestValue);
void printChildInformation  (int count, char x_coordinates[10], char y_coordinates[10], char z_coordinates[10]);
/* Signal Handling for Parent */
void initializeSignalHandler ();
void handleSIG               (int signal);
void terminateProgram        (char * fileBuffer, int pidList[MAX_CHILD]);



/* Defining Functions */

int main(int argc, char *argv[]) {
    // Creating SIGINT handler
    initializeSignalHandler();
    // File Paths   
    getFilePaths(argc, argv);
    // Getting input file properties
    int    fileSize   = getFileLength(inputFilePath);
    char * fileBuffer = (char*)calloc(fileSize, sizeof(char));
    // Getting file descriptors
    readFD   = openFile(inputFilePath, "r");
    writeFD  = openFile(outputFilePath, "w");
    // Executing parentProcess
    parentProcess(fileBuffer, fileSize);
    // Freeing the allocated variables
    free(fileBuffer);
    return 0;
}

void parentProcess(char * fileBuffer, int fileSize) {
    char x_coordinates[10], y_coordinates[10], z_coordinates[10];
    pid_t pidList[10000];

    printFirstDialogue();

    for(int fileIndex=0; ; childCount++) {
        // EOF check
        if(fileSize - fileIndex < 30) { break; }
        // Getting coordinates
        fileIndex = getCoordinates(fileBuffer, fileIndex, x_coordinates, y_coordinates, z_coordinates);
        // Printing child information
        printChildInformation(childCount, x_coordinates, y_coordinates, z_coordinates);
        // Sleeping to get signals
        //sleep(1);
        // Creating child process
        createChildProcess(pidList, x_coordinates, y_coordinates, z_coordinates);
    }

    // Waiting for children processes to terminate
    for(int i=0; i<childCount; i++) {
        if(sigintFlag == 1)
            terminateProgram(NULL, pidList);
        wait(NULL);
    }


    /* AFTER ALL CHILDREN PROCESSES RETURNED */
    float covarianceMatrices[childCount][3][3];
    float frobeniusNorm[childCount];

    // Close output file since every child finished their job    
    closeFile(writeFD);

    // Reading contents of the output file
    getCovarianceMatricesFromOutputFile(covarianceMatrices);
    // Getting frobenius norm values of matrices
    getFrobeniusNormOfMatrices(covarianceMatrices, frobeniusNorm);
    // Finding the two closest matrices respect to frobenius norm
    float closestValue = findTwoClosestMatrices(frobeniusNorm);
    // Printing the result
    printResultToConsole(covarianceMatrices, closestValue);
}







// Helper Functions

void getFilePaths(int argc, char *argv[]) {
    char * errorMsg1 = "\nWrong command line arguments. Format should be as this: ";
    char * errorMsg2 = "\"./hw2 -i inputFilePath -o outputFilePath\" or ";
    char * errorMsg3 = "\"./hw2 -o outputFilePath -i inputFilePath\"\n\n";

    if(argc != 5) {
        write(STDERR_FILENO, errorMsg1, strlen(errorMsg1));
        write(STDERR_FILENO, errorMsg2, strlen(errorMsg2));
        write(STDERR_FILENO, errorMsg3, strlen(errorMsg3));

        exit(EXIT_FAILURE);
    }
    if(strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-o") == 0) {
        inputFilePath  = argv[2];
        outputFilePath = argv[4];
    }
    else if(strcmp(argv[1], "-o") == 0 && strcmp(argv[3], "-i") == 0) {
        inputFilePath  = argv[4];
        outputFilePath = argv[2];
    }
    else {
        perror("\nWrong command line arguments. Format should be as this:\n");
        perror("(\"./hw2 -i inputFilePath -o outputFilePath\") or this:\n");
        perror("(\"./hw2 -o outputFilePath -i inputFilePath\")\n\n");
        exit(EXIT_FAILURE);
    }
}

void createChildProcess(int pidList[MAX_CHILD], char x_coordinates[10], char y_coordinates[10], char z_coordinates[10]) {
    // Turning writeFD to string
    char * strWriteFD = (char*)calloc(1, sizeof(char)); strWriteFD[0] = (char)writeFD; 
    // Creating child process
    pid_t pid = fork();
    // Give error if forking is unsuccessful 
    if(pid < 0) {
        perror("Error occured while creating child process");
        exit(1);
    }
    // Execute this if pid is belong to child process
    if(pid == 0) {
        // Assigning command line arguments for child process
        char * newArgv[] = {"./child", strWriteFD,  (char *)NULL};
        char * newArgp[] = {x_coordinates, y_coordinates, z_coordinates, (char *)NULL};
        // Executing childProcess.c executable file for child process  
        execve(newArgv[0], newArgv, newArgp);
    }

    if(sigintFlag == 1)
        terminateProgram(strWriteFD, pidList);

    // Adding child process id into pidList
    pidList[childCount] = pid;
    // Free the strWriteFD
    free(strWriteFD);
}

int getCoordinates(char * fileBuffer, int fileIndex, char * x_coordinates, char * y_coordinates, char * z_coordinates) {
    for(int coordIndex=0; coordIndex<10; coordIndex++) {
        // Reading 1 byte
        readFromFileFD(readFD, fileBuffer+fileIndex, 1);
        // Converting read char into int
        x_coordinates[coordIndex] = fileBuffer[fileIndex++];
        // Reading 1 byte
        readFromFileFD(readFD, fileBuffer+fileIndex, 1);
        // Converting read char into int
        y_coordinates[coordIndex] = fileBuffer[fileIndex++];
        // Reading 1 byte
        readFromFileFD(readFD, fileBuffer+fileIndex, 1);
        // Converting read char into int
        z_coordinates[coordIndex] = fileBuffer[fileIndex++];
    }
    return fileIndex;
} 

void getCovarianceMatricesFromOutputFile(float covarianceMatrices[childCount][3][3]) {
    // Getting output file properties
    int    fileSize   = getFileLength(outputFilePath);
    char * fileBuffer = (char*)calloc(fileSize, sizeof(char));
    // Reading all content into a buffer
    readFromFile(outputFilePath, fileBuffer, fileSize);
    // Temp matrix value buffer
    char * matrixBuffer;

    matrixBuffer = strtok(fileBuffer, " "); covarianceMatrices[0][0][0] = atof(matrixBuffer);
    matrixBuffer = strtok(NULL, " " );      covarianceMatrices[0][0][1] = atof(matrixBuffer);
    matrixBuffer = strtok(NULL, "\n");      covarianceMatrices[0][0][2] = atof(matrixBuffer);

    int child = 0, row = 1, column = 0;
    while(child != childCount) {
        // Resetting column
        column = 0;
        // Assigning matrix values into array
        matrixBuffer = strtok(NULL, " ");  covarianceMatrices[child][row][column] = atof(matrixBuffer); column++;
        matrixBuffer = strtok(NULL, " ");  covarianceMatrices[child][row][column] = atof(matrixBuffer); column++;
        matrixBuffer = strtok(NULL, "\n"); covarianceMatrices[child][row][column] = atof(matrixBuffer); column++;
        // Incrementing row
        row++;
        // Check for row
        if(row == 3) { row = 0; child++; }
    }

    // Freeing the allocated variable
    free(fileBuffer);
}

void getFrobeniusNormOfMatrices(float covarianceMatrices[childCount][3][3], float frobeniusNorm[childCount]) {
    for(int i=0; i<childCount; i++) 
        frobeniusNorm[i] = calculateFrobeniusNorm(covarianceMatrices[i]);
}

float calculateFrobeniusNorm(float covarianceMatrix[3][3]) {
    float sum = 0;

    for(int row=0; row<3; row++)
        for(int column=0; column<3; column++)
            sum += covarianceMatrix[row][column] * covarianceMatrix[row][column]; 

    return sqrt(sum); 
}

float findTwoClosestMatrices(float frobeniusNorm[childCount]) {
    float min_difference = INFINITY;
    float curr_difference = 0;

    for(int i=0; i<childCount-1; i++) {
        for(int j=i+1; j<childCount; j++) {
            // Getting current difference
            curr_difference = frobeniusNorm[i] - frobeniusNorm[j]; 
            // Making difference positive if it is lower than 0
            if(curr_difference < 0)
                curr_difference = curr_difference * (-1);
            // Comparing it with min_difference
            if(curr_difference < min_difference) {
                min_difference = curr_difference;
                firstChild = i;
                secondChild = j;
            }
        }
    }
    return min_difference;
}

void printFirstDialogue() {
    char * firstMessage = "\nProcess P reading ";
    char * newLine = "\n\n";

    writeToFileFD(STDOUT_FILENO, firstMessage);
    writeToFileFD(STDOUT_FILENO, inputFilePath);
    writeToFileFD(STDOUT_FILENO, newLine);
}

void printResultToConsole(float covarianceMatrices[childCount][3][3], float closestValue) {
    // To hold float values
    char * tempBuffer = (char*)calloc(20, sizeof(char));
    // Strings to print to the console
    char * strEOF = "\nReached EOF, collecting outputs from ";
    char * beginning = "\n\nThe closest 2 matrices are\n";
    char * divider = "--------------------------\n";
    char * ending = "and their distance is: ";

    // Starting to print

    // Ex: Reached EOF, collecting outputs from hw2/outputFile.dat
    writeToFileFD(STDOUT_FILENO, strEOF);

    writeToFileFD(STDOUT_FILENO, outputFilePath);
    writeToFileFD(STDOUT_FILENO, beginning);
    writeToFileFD(STDOUT_FILENO, divider);

    for(int row=0; row<3; row++) {
        for(int column=0; column<2; column++) {
            sprintf(tempBuffer, "%f\t", covarianceMatrices[firstChild][row][column]);
            writeToFileFD(STDOUT_FILENO, tempBuffer);
        }

        if(row == 1) {
            sprintf(tempBuffer, "%f\tand\t", covarianceMatrices[firstChild][row][2]);
            writeToFileFD(STDOUT_FILENO, tempBuffer);
        }
        else {
            sprintf(tempBuffer, "%f\t\t", covarianceMatrices[firstChild][row][2]);
            writeToFileFD(STDOUT_FILENO, tempBuffer);
        }

        for(int column=0; column<2; column++) {
            sprintf(tempBuffer, "%f\t", covarianceMatrices[secondChild][row][column]);
            writeToFileFD(STDOUT_FILENO, tempBuffer);
        }
        sprintf(tempBuffer, "%f\n", covarianceMatrices[secondChild][row][2]);
        writeToFileFD(STDOUT_FILENO, tempBuffer);
    }

    writeToFileFD(STDOUT_FILENO, divider);
    writeToFileFD(STDOUT_FILENO, ending);
    sprintf(tempBuffer, "%.3f\n\n", closestValue);
    writeToFileFD(STDOUT_FILENO, tempBuffer);

    free(tempBuffer);
}

void printChildInformation(int count, char x_coordinates[10], char y_coordinates[10], char z_coordinates[10]) {

    // String buffer to hold child information
    char * strBuffer = (char*)calloc(100, sizeof(char));
    // First printing the number of the child process
    sprintf(strBuffer, "Created R_%d with ", count); writeToFileFD(STDOUT_FILENO, strBuffer);
    // Then printing the coordinates
    for(int i=0; i<10; i++) {
        sprintf(strBuffer, "(%d,", (int)x_coordinates[i]); writeToFileFD(STDOUT_FILENO, strBuffer);
        sprintf(strBuffer, "%d," , (int)y_coordinates[i]); writeToFileFD(STDOUT_FILENO, strBuffer);
        if(i == 9) {
            sprintf(strBuffer, "%d)", (int)z_coordinates[i]); writeToFileFD(STDOUT_FILENO, strBuffer);
        }
        else {
            sprintf(strBuffer, "%d),", (int)z_coordinates[i]); writeToFileFD(STDOUT_FILENO, strBuffer);
        }
    }
    writeToFileFD(STDOUT_FILENO, "\n");

    free(strBuffer);
}


void initializeSignalHandler() {
    sigset_t sigSet;
    // Sigset to block other signals
    sigfillset(&sigSet); sigdelset(&sigSet, SIGINT); sigprocmask(SIG_BLOCK, &sigSet, NULL);
    // Sigaction struct
    struct sigaction sigActionStruct;
    // Giving it a memory space
    memset(&sigActionStruct, 0, sizeof(sigActionStruct));
    // Telling which handler function to use
    sigActionStruct.sa_handler = &handleSIG;
    // Adding it into sigaciton
    if(sigaction(SIGINT, &sigActionStruct, NULL) == -1) {
        write(STDERR_FILENO, "Signal initialization failed. Aborting the program.\n", 54);
        exit(EXIT_FAILURE);
    }
}

void handleSIG(int signal) { 
    switch (signal) {
        case SIGINT:
            sigintFlag = 1; 
            break;
        default:
            break;
    }
}

void terminateProgram(char * fileBuffer, int pidList[MAX_CHILD]) {
    for(int i=0; i<childCount; i++) {
        kill(pidList[i], SIGINT);
        wait(NULL);
    }
    // Error Dialogue
    char errorMessage1[] = "\nParent process detected SIGINT signal.\n";
    char errorMessage2[] = "Closing open files...\n";
    char errorMessage3[] = "Freeing all variables...\n";
    char errorMessage4[] = "Deleting the output file...\n";
    char errorMessage5[] = "Terminating the program...\n";
    write(STDOUT_FILENO, errorMessage1, strlen(errorMessage1)); sleep(1); 
    write(STDOUT_FILENO, errorMessage2, strlen(errorMessage2)); sleep(1);
    write(STDOUT_FILENO, errorMessage3, strlen(errorMessage3)); sleep(1);
    write(STDOUT_FILENO, errorMessage4, strlen(errorMessage4)); sleep(1);
    write(STDOUT_FILENO, errorMessage5, strlen(errorMessage5)); sleep(1);
    // Closing the files
    closeFile(writeFD);
    closeFile(readFD);
    unlink(outputFilePath);
    // Then deleting the output file
    // Freeing the bufer
    free(fileBuffer);
    exit(1);
}
