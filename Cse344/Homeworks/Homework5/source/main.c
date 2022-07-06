#include "../header/main.h"
#include "../header/file.h"
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <math.h>

char path_to_input1_file[MAX_FILE_LEN];
char path_to_input2_file[MAX_FILE_LEN];
char path_to_output_file[MAX_FILE_LEN];
int N;
int powN;
int M;

int** matrixA;
int** matrixB;
int** matrixC;
double complex** matrixDFT;

char* buffer;
char timeBuffer[MAX_TEXT_LEN];
char errBuffer[MAX_ERR_LEN];

pthread_t* threads;
index_t* indexes;

pthread_mutex_t waitLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitCond = PTHREAD_COND_INITIALIZER;
int finishedCount = 0;

struct timeval start, startB, end;

int sigintFlag = NOSIGNAL;

int main(int argc, char *argv[]) {
    // First, initialize signal handler for SIGINT signal
    initializeSignalHandler();
    // Control command line arguments
    if(sigintFlag == NOSIGNAL) ctrlcla(argc, argv);
    // Allocate space for matrix arrays
    allocateMatrices(myPow(N));
    // Start Timer
    if(sigintFlag == NOSIGNAL) gettimeofday(&start, NULL);     
    // Fill the matrices
    if(sigintFlag == NOSIGNAL) fillMatrices();
    // Inform the user
    if(sigintFlag == NOSIGNAL) printToConsole1(powN, M);
    // Allocate space for threads
    if(sigintFlag == NOSIGNAL) createThreads(); 
    // Join threads and deallocate spaces allocated for threads & thread args
    if(sigintFlag == NOSIGNAL) joinThreads();
    // Write result dft matrix to the output file
    if(sigintFlag == NOSIGNAL) saveToOutputFile();
    // Finish the timer and inform the user
    if(sigintFlag == NOSIGNAL) gettimeofday(&end, NULL);
    if(sigintFlag == NOSIGNAL) printToConsole5(getElapsedTime(start, end));
    // Terminate program if there is sigint signal
    if(sigintFlag == SIGNAL) terminateProgram();
    // Deallocate the allocated matrix spaces

    deallocateMatrices(myPow(N));

    return 0;
}

void ctrlcla(int argc, char *argv[]) {
    if(argc != 11)
        exitWithError("Argument count must be 11.\n");

    int check1 = strcmp(argv[1], "-i");
    int check2 = strcmp(argv[3], "-j");
    int check3 = strcmp(argv[5], "-o");
    int check4 = strcmp(argv[7], "-n");
    int check5 = strcmp(argv[9], "-m");

    if(check1 != 0 || check2 != 0 || check3 != 0 || check4 != 0 || check5 != 0)
        exitWithError("Not valid command line arguments. \nCorrect example: \"./hw5 -i \"file1.txt\" -j \"file2.txt\" -o \"output.txt\" -n 4 -m 2\"\n");

    strcpy(path_to_input1_file, argv[2]);
    strcpy(path_to_input2_file, argv[4]);
    strcpy(path_to_output_file, argv[6]);
    N = atoi(argv[8]); powN = myPow(N);
    M = atoi(argv[10]);

    if(N <= 2)
        exitWithError("n value must be bigger than 2.\n");
    if(M < 2)
        exitWithError("M must be bigger than 2.\n");
    if(M > powN)
        exitWithError("M should be less than 2^N.\n");
}



void allocateMatrices() {
    matrixDFT = (double complex**)malloc(sizeof(double complex*) * powN);
    matrixA = (int**)malloc(sizeof(int*) * powN);
    matrixB = (int**)malloc(sizeof(int*) * powN);
    matrixC = (int**)malloc(sizeof(int*) * powN);
    buffer  = (char*)malloc(sizeof(char) * powN*powN);
    

    for(int i=0; i<powN; i++) {
        matrixA[i] = (int*)malloc(sizeof(int) * powN);    
        matrixB[i] = (int*)malloc(sizeof(int) * powN);    
        matrixC[i] = (int*)malloc(sizeof(int) * powN);    
        matrixDFT[i]  = (double complex*)malloc(sizeof(double complex) * powN);
    }
}

void deallocateMatrices() {
    for(int i=0; i<powN; i++) {
        free(matrixA[i]);
        free(matrixB[i]);
        free(matrixC[i]);
        free(matrixDFT[i]);
    }

    free(matrixDFT);
    free(matrixA);
    free(matrixB);
    free(matrixC);
    free(buffer);
}



void createThreads() {
    // Allocate space for threads
    threads = (pthread_t*)malloc(sizeof(pthread_t) * M);
    // Allocate space for thread args
    indexes = (index_t*)malloc(sizeof(index_t) * M); 
    // Give initial values to indexes
    int gap = powN / M;
    for(int i=0; i<M; i++) {
        indexes[i].start = i*gap;
        indexes[i].end = (i*gap) + gap - 1;
        indexes[i].id = i+1;
    }
    if(indexes[M-1].end != powN - 1)
        indexes[M-1].end = powN - 1;
    // Creating threads
    for(int i=0; i<M; i++) 
        pthread_create(&threads[i], NULL, threadTasks, (void*)(&indexes[i]));
}

void joinThreads() {
    // Declare complex objects to store thread return values
    double complex** tempMatrices = (double complex**)malloc(M * sizeof(double complex*));
    // Join all threads
    for(int i=0; i<M; i++) {
        pthread_join(threads[i], (void**)(&tempMatrices[i]));
        tempMatrices[i] = (double complex*)tempMatrices[i];
    }
    tempMatrices = (double complex**)tempMatrices;
    // Transport tempMatrices values to matrixDFT

    fromTempToDFT(tempMatrices);
    // Deallocate allocated spaces for threads and indexes
    free(threads);
    free(indexes);
}



void fillMatrices() {
    // Read input1.txt file
    if(readFromFile(path_to_input1_file, buffer, powN*powN) != powN*powN)
        exitWithError("Fatal error occured. First input file contains unsufficient content.\n");
    // Fill matrixA
    fromBufferToArray(matrixA);
    // Read input2.txt file
    if(readFromFile(path_to_input2_file, buffer, powN*powN) != powN*powN)
        exitWithError("Fatal error occured. Second input file contains unsufficient content.\n");
    // Fill matrixA
    fromBufferToArray(matrixB);
}

void fromBufferToArray(int** array) {
    int index = 0;

    for(int i=0; i<powN; i++) 
        for(int j=0; j<powN; j++, index++)     
            array[i][j] = (int)buffer[index];
}

void fromTempToDFT(double complex ** tempArr) {
    int index = 0;

    for(int i=0; i<M; i++) {
        index = 0;
        for(int row=0; row<powN; row++) 
            for(int column=indexes[i].start; column<=indexes[i].end; column++, index++) 
                matrixDFT[row][column] = tempArr[i][index];
    }

    // Free allocated space for temp array
    for(int i=0; i<M; i++)
        free(tempArr[i]);
    free(tempArr);
}


void* threadTasks(void* arg) {
    // First cast arg to index_t
    index_t limit = *((index_t*)arg);
    // Then calculate matrixC
    getMatrixC(limit);
    // Wait other threads or broadcast if it is the last one
    waitOthersOrBroadcast(limit.id);
    // Start the next timer
    gettimeofday(&startB, NULL);
    // Allocate space for 2D Fourier Transform applied matrix
    int row = powN, column = limit.end - limit.start + 10;
    double complex* dft = allocateDftMatrix(row, column);
    // Calculate the 2D Fourier Transform
    calculate2DFourierTransform(dft, limit);
    // Finish second timer and inform the user
    gettimeofday(&end, NULL);
    printToConsole4(limit.id, getElapsedTime(startB, end));
    // Return the 2D discrete fourier transform
    return (void*)dft;
}

void getMatrixC(index_t limit) {
    // Calculate matrixC between indexes start and end
    for(int row = 0; row < powN; row++) {
        for(int column = limit.start; column <= limit.end; column++) {
            matrixC[row][column] = productOfOneElement(row, column);
        }
    }
}

int productOfOneElement(int row, int column) {
    int result = 0;

    for(int i=0; i<powN; i++) 
        result += matrixA[row][i] * matrixB[i][column];

    return result;
}

void waitOthersOrBroadcast(int threadID) {
    // Inform the user
    gettimeofday(&end, NULL);
    printToConsole2(threadID, getElapsedTime(start, end));
    // Lock the mutex to enter critical region
    if(pthread_mutex_lock(&waitLock) != 0)
        exitWithError("Error occured while executing pthread_mutex_lock.\n");
    // Increment the finished thread count
    finishedCount++;     // Wait until everybody finishes
    while(finishedCount < M)
        pthread_cond_wait(&waitCond, &waitLock);
    // broadcast since this is the last thread
    pthread_cond_broadcast(&waitCond);
    // Inform the user
    printToConsole3(threadID);
    // Release the lock since we finished our job in critical region
    pthread_mutex_unlock(&waitLock);
}

double complex* allocateDftMatrix(int rowSize, int columnSize) {
    // Allocate matrix
    double complex* newDft = (double complex*)malloc((rowSize * columnSize) * sizeof(double complex));
    // Return the matrix
    return newDft;
}

void printDFT(double complex dft) {
    printf("%.3f + %.3fi\n", creal(dft), cimag(dft));
}

void calculate2DFourierTransform(double complex* dft, index_t limit) {
    int index = 0;
    for(int k=0; k<powN; k++) {
        for(int l=limit.start; l<=limit.end; l++, index++) {
            double realPart = 0;
            double imagPart = 0;
            for(int m=0; m<powN; m++) {
                for(int n=0; n<powN; n++) {
                    // Power value for e 
                    double part1 = (-2) * (M_PI) * k * m /(double)powN;
                    double part2 = (-2) * (M_PI) * l * n /(double)powN;
                    // f(m,n)
                    double complex f_mn = matrixC[m][n]; 

                    realPart += f_mn * cos(part1 + part2);                    
                    imagPart += f_mn * 1.0 * sin(part1 + part2);                  
                }
            }
            dft[index] = realPart + imagPart * I;
        }
    }
}

int myPow(int number) {
    int result = 1;

    for(int i=0; i<number; i++)
        result *= 2;

    return result;
}

void printMatrix(int** matrix) {
    for(int i=0; i<powN; i++) {
        for(int j=0; j<powN; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

char* getTimeBuffer() {
    time_t rawtime;
    struct tm * timeinfo;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
	int day   = timeinfo->tm_mday;		
	int month = timeinfo->tm_mon + 1;
	int year  = timeinfo->tm_year + 1900;
	int hour  = timeinfo->tm_hour;
	int min   = timeinfo->tm_min; 
	int sec   = timeinfo->tm_sec;


	sprintf(timeBuffer, "%d/%d/%d | %d:%d:%d -> ", day, month, year, hour, min, sec);
	return timeBuffer;
}

float getElapsedTime(struct timeval startTime, struct timeval endTime) {
    float microseconds = (float)(endTime.tv_usec - startTime.tv_usec) / 1000000.0;
    float seconds      = (float)(endTime.tv_sec - startTime.tv_sec); 

    return microseconds + seconds;
}


void printToConsole1(int dimension, int threadCount) {
    char textBuffer[MAX_TEXT_LEN];
    
    sprintf(textBuffer, "%sTwo matrices of size %dx%d have been read. The number of threads is %d\n", 
            getTimeBuffer(),
            dimension, 
            dimension,
            threadCount);

    writeToFileFD(STDOUT_FILENO, textBuffer, strlen(textBuffer));
}

void printToConsole2(int threadID, float elapsedTime) {
    char textBuffer[MAX_TEXT_LEN];
    
    sprintf(textBuffer, "%sThread %d has reached the rendezvous point in %f seconds.\n", 
            getTimeBuffer(),
            threadID,
            elapsedTime);

    writeToFileFD(STDOUT_FILENO, textBuffer, strlen(textBuffer));
}

void printToConsole3(int threadID) {
    char textBuffer[MAX_TEXT_LEN];
    
    sprintf(textBuffer, "%sThread %d is advancing to the second part.\n", 
            getTimeBuffer(),
            threadID);

    writeToFileFD(STDOUT_FILENO, textBuffer, strlen(textBuffer));
}

void printToConsole4(int threadID, float elapsedTime) {
    char textBuffer[MAX_TEXT_LEN];
    
    sprintf(textBuffer, "%sThread %d has finished the second part in %f seconds.\n", 
            getTimeBuffer(),
            threadID,
            elapsedTime);

    writeToFileFD(STDOUT_FILENO, textBuffer, strlen(textBuffer));
}

void printToConsole5(float elapsedTime) {
    char textBuffer[MAX_TEXT_LEN];
    
    sprintf(textBuffer, "%sThe process has written to the output file. The total time spent is %f seconds\n", 
            getTimeBuffer(),
            elapsedTime);

    writeToFileFD(STDOUT_FILENO, textBuffer, strlen(textBuffer));
}

void saveToOutputFile() {
    // Get delimiter to seperate DFT values
    char delimiter[1] = ",";
    // Get file descriptor for the output file with WR_ONLY | WR_APPEND permissions
    int writeFD = openFile(path_to_output_file, "w");
    // Write DFT values to the file
    for(int i=0; i<powN; i++) {
        for(int j=0; j<powN; j++) {
            if(j == powN - 1)
                sprintf(buffer, "%.3f%+.3fi", creal(matrixDFT[i][j]), cimag(matrixDFT[i][j]));
            else
                sprintf(buffer, "%.3f%+.3fi%c", creal(matrixDFT[i][j]), cimag(matrixDFT[i][j]), delimiter[0]);
            writeToFileFD(writeFD, buffer, strlen(buffer));
        }
        writeToFileFD(writeFD, "\n", 1);
    }
    closeFile(writeFD);
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
            sigintFlag = SIGNAL; 
            break;
        default:
            break;
    }
}

void terminateProgram() {
    // Inform the user about SIGINT signal
    sprintf(errBuffer, "SIGINT signal detected. Terminating program after cleaning.\n");
    writeToFileFD(STDERR_FILENO, errBuffer, strlen(errBuffer));
    // Deallocate matrices and buffers
    deallocateMatrices();
    // Remove created file
    unlink(path_to_output_file);
    // Exit the program
    exit(EXIT_FAILURE);
}

/* End */