/* Libraries */
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "file.h"
/* Define Values */
#define NUM_OF_POINTS 10
/* Global Variables */
static int sigintFlag = 0;

/* Main Functions */
void getCoordinatesFromEnvVar    (char *argp[], int * x_coordinates, int * y_coordinates, int * z_coordinates);
void calculateCovarianceMatrix   (float covarianceMatrix[3][3], int * x_coordinates, int * y_coordinates, int * z_coordinates);
void writeCovarianceMatrixToFile (int writeFD, float covarianceMatrix[3][3]);
/* Helper Functions */
float getMean       (int numbers[NUM_OF_POINTS]);
float getVariance   (int numbers[NUM_OF_POINTS], float mean);
float getCovariance (int firstNumbers[NUM_OF_POINTS], int secondNumbers[NUM_OF_POINTS], float meanOfFirst, float meanOfSecond);
/* Signal Handler */
void initializeSignalHandler ();
void handleSignal            (int signal);
void terminateChild          (char * buffer);


/* Defining Functions */

int main(int argc, char *argv[], char *argp[]) {
    // Creating SIGINT handler
    initializeSignalHandler();
    // Getting the writeFD
    int writeFD = (int)argv[1][0];
    // Locking the file for writing
    lockFileForWrite(writeFD);
    // X, Y and Z coordinates
    int x_coordinates[NUM_OF_POINTS], y_coordinates[NUM_OF_POINTS], z_coordinates[NUM_OF_POINTS];
    // Matrix that will be writen to output file
    float covarianceMatrix[3][3];


    // Gets x, y and z coordinates from environment variables and assigns them to local variables
    getCoordinatesFromEnvVar(argp, x_coordinates, y_coordinates, z_coordinates);
    // Calculates covarience matrix using x,y and z coordinates
    calculateCovarianceMatrix(covarianceMatrix, x_coordinates, y_coordinates, z_coordinates);
    // Writing covariance matrix into output file with given write file descriptor
    writeCovarianceMatrixToFile(writeFD, covarianceMatrix);
    // Unlocking the file
    unlockFileForWrite(writeFD);

    return 0;
}

void getCoordinatesFromEnvVar(char *argp[], int * x_coordinates, int * y_coordinates, int * z_coordinates) {
    for(int i=0, x=0, y=0, z=0; i<NUM_OF_POINTS*3; i++) {
        if(i >= 0 && i < NUM_OF_POINTS) {
            x_coordinates[x] = (int)argp[0][x]; x++;
        }
        else if(i >= NUM_OF_POINTS && i < NUM_OF_POINTS*2) {
            y_coordinates[y] = (int)argp[1][y]; y++;
        }
        else if(i >= NUM_OF_POINTS*2 && i < NUM_OF_POINTS*3) {
            z_coordinates[z] = (int)argp[2][z]; z++;
        }
    }
}

void writeCovarianceMatrixToFile(int writeFD, float covarianceMatrix[3][3]) {
    char * strHolder = (char*)calloc(20, sizeof(char));
    // Writing the first row
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            sprintf(strHolder, "%f", covarianceMatrix[i][j]);
            writeToFileFD(writeFD, strHolder);
            writeToFileFD(writeFD, " ");
            
        }
        writeToFileFD(writeFD, "\n");
    }
    //sleep(1);
    writeToFileFD(writeFD, "\n");
    
    if(sigintFlag == 1 )
        terminateChild(strHolder);

    free(strHolder);
}





/* Covariance Calculation */

void calculateCovarianceMatrix(float covarianceMatrix[3][3], int * x_coordinates, int * y_coordinates, int * z_coordinates) {
    // Mean values of x,y and z coordinates
    float meanX, meanY, meanZ;
    // Calculating mean of coordinates
    meanX = getMean(x_coordinates);
    meanY = getMean(y_coordinates);
    meanZ = getMean(z_coordinates);
    // Variance of coordinates
    float varX, varY, varZ;
    // Calculating the variance
    varX = getVariance(x_coordinates, meanX);
    varY = getVariance(y_coordinates, meanY);
    varZ = getVariance(z_coordinates, meanZ);
    // Covariance of coordinates 
    float covXY, covXZ, covYZ;
    // Calculating the covariance
    covXY = getCovariance(x_coordinates, y_coordinates, meanX, meanY);
    covXZ = getCovariance(x_coordinates, z_coordinates, meanX, meanZ);
    covYZ = getCovariance(y_coordinates, z_coordinates, meanY, meanZ);
    
    // Now placing the results into covarianceMatrix 2d array
    
    // First variance results
    covarianceMatrix[0][0] = varX;
    covarianceMatrix[1][1] = varY;
    covarianceMatrix[2][2] = varZ;
    // Then covariance results
    // XY
    covarianceMatrix[0][1] = covXY;
    covarianceMatrix[1][0] = covXY;
    // XZ
    covarianceMatrix[0][2] = covXZ;
    covarianceMatrix[2][0] = covXZ;
    // YZ
    covarianceMatrix[1][2] = covYZ;
    covarianceMatrix[2][1] = covYZ;
}

float getMean(int numbers[NUM_OF_POINTS]) {
    int sum = 0;
    
    for(int i=0; i<NUM_OF_POINTS; i++) {
        sum += numbers[i];
    }

    return (float)sum / (float)NUM_OF_POINTS;
}

float getVariance(int numbers[NUM_OF_POINTS], float mean) {
    float remain = 0, sum = 0;
    
    for(int i=0; i<NUM_OF_POINTS; i++) {
        // First we substract numbers[i] from mean value
        remain = numbers[i] - mean;
        // Then, we add the square of it to sum
        sum += remain * remain;
    }
    // Finally we divide sum with n
    float result = sum / NUM_OF_POINTS;
    // Returning the result 
    return result;
}

float getCovariance(int firstNumbers[NUM_OF_POINTS], int secondNumbers[NUM_OF_POINTS], float meanOfFirst, float meanOfSecond) {
    float remainFirst = 0, remainSecond = 0;
    float sum = 0;

    for(int i=0; i<NUM_OF_POINTS; i++) {
        // First we remove mean values from numbers[i]
        remainFirst  = firstNumbers[i]  - meanOfFirst;
        remainSecond = secondNumbers[i] - meanOfSecond;
        // Then we add product of these two numbers into sum
        sum += remainFirst * remainSecond;
    }
    // Finally we divide sum with n
    float result = sum / NUM_OF_POINTS;
    // Returning the result 
    return result;
}

void initializeSignalHandler() {
    sigset_t sigSet;
    // Sigset to block other signals
    sigfillset(&sigSet); sigdelset(&sigSet, SIGINT); sigprocmask(SIG_BLOCK, &sigSet, NULL);
    // Sigaction struct
    struct sigaction sg;
    // Giving it a memory space
    memset(&sg, 0, sizeof(sg));
    // Telling which handler function to use
    sg.sa_handler = &handleSignal;
    // Adding signals to handle
    if(sigaction(SIGINT, &sg, NULL) == -1) {
        write(STDERR_FILENO, "Signal initialization failed. Aborting the program.\n", 54);
        exit(EXIT_FAILURE);
    }
}

void handleSignal(int signal) { 
    switch (signal) {
    case SIGINT:
        sigintFlag = 1; 
        break;
    default:
        break;
    }
}

void terminateChild(char * buffer) {
    //sleep(1);
    char * strpid = (char*)calloc(20, sizeof(char));
    sprintf(strpid, "%d\n", getpid());
    // Checking
    char terminateMessage1[] = "\nA child process detected SIGINT signal. Child process id : ";
    char terminateMessage2[] = "Freeing allocated variables inside child process...\n";

    // Information message    
    write(STDOUT_FILENO, terminateMessage1, strlen(terminateMessage1));
    write(STDOUT_FILENO, strpid, strlen(strpid));
    write(STDOUT_FILENO, terminateMessage2, strlen(terminateMessage2));

    // Freeing part
    free(buffer);
    free(strpid);

    exit(1);
}
