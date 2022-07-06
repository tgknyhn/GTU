// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include "../header/fifo.h"
#include "../header/file.h"
#include "../header/matrix.h"
#include "../header/clientX.h"

// ** Global Variables **

// To measure elapsed time
struct timeval start;
struct timeval end;
// Response & Request structs
request_t request;
response_t response;
// Arguments which come from command line
char path_to_client_fifo[CLIENT_FIFO_LENGTH];
char path_to_server_fifo[MAX_FILE_LEN];
char path_to_data_file[MAX_FILE_LEN];
// File descriptors
int serverFD;
int clientFD;
// Indicates whether SIGINT signal detected or not
int sigintFlag = NO_SIGNAL;

// ** Global Variables - end **


int main(int argc, char* argv[]) {
	// Initializing the timer
	gettimeofday(&start, NULL);
	// Reads the command line arguments and assigns them into local variables
	control_cla(argc, argv);
	// Initializing the signal handler
	initializeSignalHandler();

	initializeClientFIFO();

	

	printRequestInfo();

	sendRequest(request);

	receiveResponse(response);

	gettimeofday(&end, NULL);
	
	printResponseInfo();

	if(sigintFlag == SIGNAL)
		terminateProgram();

	return 0;
}

// Reads the command line arguments and assigns them into local variables
void control_cla(int argc, char* argv[]) {
	// First controlling argument count
	if(argc != 5)
		exitWithError("Wrong argument count for client. Correct cla should contain 5 arguments.\n");
	// Then controlling arguments
	if(strcmp(argv[1], "-s") == 0 && strcmp(argv[3], "-o") == 0) {
		strcpy(path_to_server_fifo, argv[2]);
		strcpy(path_to_data_file, argv[4]);
	}
	else if(strcmp(argv[1], "-o") == 0 && strcmp(argv[3], "-s") == 0) {
		strcpy(path_to_server_fifo, argv[4]);
		strcpy(path_to_data_file, argv[2]);
	}
	else
		exitWithError("Wrong cla format. Proper one should be: \"./client -s path_to_server_fifo -o path_to_data_file\"\n");
}

void removeFifos() {
    unlink(path_to_client_fifo);
}

void initializeClientFIFO() {
	// 1) Creating client FIFO
	umask(0);
	snprintf(path_to_client_fifo, CLIENT_FIFO_LENGTH, CLIENT_FIFO_TEMPLATE, (long)getpid());

	if(mkfifo(path_to_client_fifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
		exitWithError("\nClientX | Error occured while creating client FIFO : ");

	// 2) Assigning delete function to remove FIFO files after communication is done
	if(atexit(removeFifos) != 0)
		exitWithError("\nClientX | Error occured while removing the FIFO files : ");
}

void sendRequest() {
	// 1) Defining the request variables
	request.pid = getpid();
	strcpy(request.pathToDataFile, path_to_data_file);

	// 2) Opening the server FIFO 
	serverFD = open(path_to_server_fifo, O_WRONLY);

	if(serverFD == -1 && sigintFlag == NO_SIGNAL) 
		exitWithError("\nClientX | Error occured while opening the server FIFO : ");

	if(sigintFlag == SIGNAL)
		return;

	// 3) Sending the request
	if(write(serverFD, &request, sizeof(request_t)) != sizeof(request_t))
		if(sigintFlag == NO_SIGNAL)
			exitWithError("\nClientX | Error occured while sending the request.");
}

void receiveResponse() {
	if(sigintFlag == SIGNAL)
		return;
	// 6) Opening the client FIFO
	clientFD = open(path_to_client_fifo, O_RDONLY);

	if(clientFD == -1 && sigintFlag == NO_SIGNAL)
		exitWithError("\nClientX | Error occured while opening the client FIFO.");



	// 7) Reading the response
	if(read(clientFD, &response, sizeof(response_t)) != sizeof(response_t)) {
		if(sigintFlag == NO_SIGNAL)
			exitWithError("\nClientX | Error occured while reading the response.");
		else
			return;
	}
	

	close(serverFD);
	close(clientFD);

}

void printTimeAndPID() {
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

	printf("%d/%d/%d | %d:%d:%d -> Client PID#%d", day, month, year, hour, min, sec, getpid());
}

void printRequestInfo() {
	int size = get_matrix_size(path_to_data_file);
	// First we print time
	printTimeAndPID();
	// Then other informations
	printf(" (%s) is submitting a %dx%d matrix\n", path_to_data_file, size, size);
}

void printResponseInfo() {
	if(sigintFlag == SIGNAL)
		return;

	long second = (end.tv_sec - start.tv_sec);
	long microsec = ((second*1000000) + end.tv_usec) - (start.tv_usec);
	long remainingMillisec = (microsec / 1000) % (second * 1000);

	// First we print time
	printTimeAndPID();
	// Then other informations
	if(response.isInvertible == INVERTIBLE)
		printf(": the matrix is invertible, total time %ld.%ld seconds, goodbye.\n", second, remainingMillisec / 10);
	else
		printf(": the matrix is not invertible, total time %ld.%ld seconds, goodbye.\n", second, remainingMillisec / 10);
}

void initializeSignalHandler() {
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

void terminateProgram() {
	printf("\n");
	printTimeAndPID();
	printf(" SIGINT signal detected. Terminating the program.\n");
	// Closing opened files
	close(serverFD);
	close(clientFD);
}



// End //