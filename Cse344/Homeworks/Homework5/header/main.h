#ifndef MAIN_H
#define MAIN_H

#include <sys/time.h> // Necessary for getElapsedTime function
#include <complex.h>  // Necessary for 2D Discrete Fourier Transform calculation

#define SIGNAL 1
#define NOSIGNAL 0
#define MAX_ERR_LEN 100

typedef struct index_s {
    int start;
    int end;
    int id;
} index_t;

void ctrlcla(int argc, char *argv[]);

void allocateMatrices();
void deallocateMatrices();

void createThreads();
void joinThreads();

void fillMatrices();
void fromBufferToArray(int** array);
void fromTempToDFT(double complex ** tempArr);


void* threadTasks(void* arg);
void  getMatrixC(index_t limit);
int   productOfOneElement(int row, int column);
void  waitOthersOrBroadcast(int threadID);

double complex* allocateDftMatrix(int rowSize, int columnSize);
void calculate2DFourierTransform(double complex* dft, index_t limit);

int myPow(int number);
char* getTimeBuffer();
float getElapsedTime(struct timeval startTime, struct timeval endTime);

void printMatrix(int** matrix);
void printToConsole1(int dimension, int threadCount);
void printToConsole2(int threadID, float elapsedTime);
void printToConsole3(int threadID);
void printToConsole4(int threadID, float elapsedTime);
void printToConsole5(float elapsedTime);
void saveToOutputFile();

void initializeSignalHandler();
void handleSIG(int signal);
void terminateProgram();

#endif