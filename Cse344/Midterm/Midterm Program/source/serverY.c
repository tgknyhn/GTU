// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <sys/mman.h>
#include <semaphore.h>
#include "../header/fifo.h"
#include "../header/file.h"
#include "../header/matrix.h"
#include "../header/daemon.h"
#include "../header/serverY.h"

// ** Global Variables **

// Busy Counters
int* busyCountY_sm;
int* busyCountZ_sm;
// Structures
request_t request;
response_t response;
result_t result;
result_t resultZ;
// ServerZ shared memory
result_t* result_sm;
result_t* resultZ_sm;
request_t* requestQueue_sm;
int* inQueue_sm;
// process id's
pid_t workersY[QUEUE_LEN];
pid_t workersZ[QUEUE_LEN];
pid_t serverZ;
pid_t workerQ; 
// Shared Memory Semaphores
sem_t* sem_workerZ;
sem_t* sem_queue;
sem_t* sem_available_child;
// Arguments which come from command line
char path_to_client_fifo[CLIENT_FIFO_LENGTH];
char path_to_server_fifo[MAX_FILE_LEN];
char path_to_log_file[MAX_FILE_LEN];
int  pool_size  = 0;
int  pool_size2 = 0;
int  sleep_time = 0;
// File descriptors
int serverFD, clientFD, dummyFD, childCountFD;
// Buffer to write into log file
char buffer[250];
// Pipes
int pfd[2];
int Zpfd[2];
int Qpfd[2];
// Indicates whether SIGINT signal detected or not
int sigintFlag = NO_SIGNAL;
// Shared memory preferences
int protection = PROT_READ | PROT_WRITE;
int visibility = MAP_SHARED | MAP_ANONYMOUS;

// ** Global Variables - end - **



int main(int argc, char* argv[]) {
	// Reads the command line arguments and assigns them into local variables
	control_cla(argc, argv);
	// Control to prevent double instance
	preventDoubleInstance(SERVERY_FILE);
	// Becoming Daemon
	becomeDaemon(BD_NO_CHDIR | BD_NO_CLOSE_FILES);
	
	// Initializing the server
	initializeSignalHandler();
	initializeQueue();
	initializeSemaphores();
	initializeResultStruct();
	initializeServerFIFO(); 
	initializeDummyFIFO();
	initializeChildCountFile();

	// SIGINT check
	if(sigintFlag == SIGNAL) { terminateServerY(); return 0; }

	// After becoming daemon, printing that serverY is started
	sprintf(buffer, "%sServer Y (%s, p=%d, t=%d) started\n", getTimeAndPID(), path_to_log_file, pool_size, sleep_time); 
	writeToFile_wFlag(path_to_log_file, buffer, "w");
	
	
	// Creating the pipes
	if(pipe(pfd) == -1)
		exitWithErrorServer("\nServerY | Error occured while creating the pipe\n");
	if(pipe(Zpfd) == -1)
		exitWithErrorServer("\nServerY | Error occured while creating the pipe for serverZ\n");
		
	// Starting server Z
	startServerZ(); if(sigintFlag == SIGNAL) { return 0; }
	// Starting Queue
	startQueueWorker(); if(sigintFlag == SIGNAL) { return 0; }
	// Creating children processes
	int pid = createChildrenProcesses();


	for(;;) {
		if(sigintFlag == SIGNAL)
			break;
		if(pid != 0)
			parentProcessY(serverFD, request);
		else 
			childProcessY(request, response, clientFD);
	}

	// Code reaches here only when SIGINT signal is detected
	if(pid != 0 && pid != serverZ) {
		// First sending SIGINT signal to our children processes
		for(int i=0; i<pool_size; i++) {
			kill(workersY[i], SIGINT);
		}
		// And SIGINT to serverZ workers
		for(int i=0; i<pool_size2; i++)
			kill(workersZ[i], SIGINT);
		// Then sending SIGINT to serverZ and queueWorker
		kill(serverZ, SIGINT);
		kill(workerQ, SIGINT);
		// At last terminating program
		terminateServerY();
	}

	return 0;
}

// Constrols command line arguments
void control_cla(int argc, char* argv[]) {
	// First controlling argument count
	if(argc != 11)
		exitWithError("Wrong argument count for serverY. Correct cla should contain 11 arguments.\n\"");
	if(strcmp(argv[1], "-s") != 0 || strcmp(argv[3], "-o") != 0 || strcmp(argv[5], "-p") != 0 || strcmp(argv[7], "-r") != 0 || strcmp(argv[9], "-t") != 0)
		exitWithError("Wrong argument syntax. Proper one should be as: \"./serverY -s '/tmp/ServerFIFO' -o 'result.log' -p 5 -r 4 -t 5\"\n\"");
	// Then assigning arguments into global variables
	strcpy(path_to_server_fifo, argv[2]);
	strcpy(path_to_log_file, argv[4]);
	pool_size  = atoi(argv[6]);
	pool_size2 = atoi(argv[8]);

	if(pool_size < 2 || pool_size2 < 2)
		exitWithError("Pool sizes can't be lower than 2.\n\"");
	
	sleep_time = atoi(argv[10]);
}

void preventDoubleInstance(char* temp_path) {
    // First trying to open 
    int open_result = open(temp_path, O_CREAT | O_EXCL);
    // Sending SIGINT signal if file is already there
    if(open_result == -1) 
		exitWithErrorServer("Another ServerY process is already running. You're not allowed to open second one.\n");
	// Reseting log file
	writeToFile(path_to_log_file, "");
	// Assigning a file to execute when program terminates
	atexit(unlinkServer);
}

/* Initializing part */

void initializeQueue() {
	// Adding addresses into shared memory
	requestQueue_sm = (request_t*)mmap(NULL, sizeof(request_t) + 10, protection, visibility, -1, 0);
	inQueue_sm = (int*)mmap(NULL, sizeof(int), protection, visibility, -1, 0);
}

void initializeSemaphores() {
	/* We initialize the semaphore counter to 1 (INITIAL_VALUE) */
    sem_workerZ 		= (sem_t*)mmap(0, sizeof(sem_t), protection, visibility, 0, 0 );
    sem_available_child = (sem_t*)mmap(0, sizeof(sem_t), protection, visibility, 0, 0 );
    sem_queue 			= (sem_t*)mmap(0, sizeof(sem_t), protection, visibility, 0, 0 );

	sem_init(sem_workerZ, 1, 0);
	sem_init(sem_available_child, 1, 0);
	sem_init(sem_queue, 1, 0);
	

	if (sem_workerZ == SEM_FAILED || sem_available_child == SEM_FAILED || sem_queue == SEM_FAILED) {
		if(errno != EEXIST) {
			
			exitWithErrorServer("Error occured while initializing the semaphores\n");
		}
    }
}

void initializeDummyFIFO() {
	// Opening dummy FIFO
	dummyFD = open(path_to_server_fifo, O_WRONLY);

	if(dummyFD == -1)
		exitWithErrorServer("\nServerY | Error occured while opening dummy FIFO\n");

	// SIGNAL control
	if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		exitWithErrorServer("\nServerY | Error occured while executing signal function\n");
}

void initializeServerFIFO() {
	// Creating Server FIFO	
	if(mkfifo(path_to_server_fifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
		exitWithErrorServer("\nServerY | Error occured while creating server FIFO\n");

	// Opening the server FIFO
	serverFD = open(path_to_server_fifo, O_RDONLY);

	if(serverFD == -1)
		if(sigintFlag == NO_SIGNAL)
			exitWithErrorServer("\nServerY | Error occured while opening server FIFO\n");
}

void initializeResultStruct() {
	// Initializing the global result struct
	result.total = 0;
	result.invertible = 0;
	result.nonInvertible = 0;
	resultZ.total = 0;
	resultZ.invertible = 0;
	resultZ.nonInvertible = 0;
	// Shared memory preferences
	int size = sizeof(result_t) + 10;
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANONYMOUS;
	// Initializing the shared memory for result struct
	result_sm = (result_t*)mmap(NULL, size, protection, visibility, -1, 0);
	resultZ_sm = (result_t*)mmap(NULL, size, protection, visibility, -1, 0);
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
		exitWithErrorServer("Signal initialization failed. Aborting the program.\n");
    }
}

void initializeChildCountFile() {
	// Initializing shared memory variables
	busyCountY_sm = (int*)mmap(NULL, sizeof(int), protection, visibility, -1, 0);
	busyCountZ_sm = (int*)mmap(NULL, sizeof(int), protection, visibility, -1, 0);

	// Giving their initial values
	busyCountY_sm[0] = 0;
	busyCountZ_sm[0] = 0;
}



// Creates pool_size children processes 
int createChildrenProcesses() {
	// Creating child processes
	int pid = fork();

	for(int child = 0; child < pool_size; child++) {
		switch (pid) {
			// Error occured
			case -1:
				exitWithErrorServer("\nServerY | Error occured while executing the fork\n");
				break;
			// Child process
			case 0:
				workersY[child] = getpid();
				break;
			// Parent process
			default: 
				pid = fork();
				break;
		}
	}

	return pid;
}


/* Parent processes */

void sendForwardingMessage() {
	// Forwarding request of client PID#700 to serverZ, matrix size 3x3, pool busy 4/4
	int size = get_matrix_size(request.pathToDataFile);
	sprintf(buffer, "%sForwarding request of client PID#%d to serverZ, matrix size %dx%d, pools busy %d/%d\n",
			getTimeAndPID(),
			request.pid,
			size, size,
			busyCountY_sm[0],
			pool_size);
	writeToFile_wFlag(path_to_log_file, buffer, "wop");
}

void parentProcessY() {
	// Reading the request
	if(read(serverFD, &request, sizeof(request_t)) != sizeof(request_t)) {
		if(sigintFlag == NO_SIGNAL)
			exitWithErrorServer("ServerY | Error occured while reading the request.");
		else
			return;
	}
	
	if(busyCountY_sm[0] >= pool_size) {
		sendForwardingMessage();
		if(write(Zpfd[WRITE], &request, sizeof(request_t)) != sizeof(request_t)) {
			if(sigintFlag == NO_SIGNAL)
				exitWithErrorServer("ServerY | Error occured while sending request to ServerZ\n");
			else
				return;
		}
	}

	// Sending it to the child process
	else {
		if(write(pfd[WRITE], &request, sizeof(request_t)) != sizeof(request_t)) {
			if(sigintFlag == NO_SIGNAL)
				exitWithErrorServer("ServerY | Error occured while sending request to worker process\n");
			else
				return;
		}
	}
}

void parentProcessZ() {
	// Reading the request
	if((read(Zpfd[READ], &request, sizeof(request_t)) != sizeof(request_t)) && errno != EEXIST) {
		if(sigintFlag == NO_SIGNAL)
			exitWithErrorServer("ServerZ | Error occured while reading the request from ServerY\n");
		else
			return;
	}
	
	addToQueue(request);
}


/* Child(worker) processes */

void childProcessY() {
	// Reading the request from parent process
	if((read(pfd[READ], &request, sizeof(request_t)) != sizeof(request_t)) && errno != EEXIST) {
		if(sigintFlag == NO_SIGNAL)
			exitWithErrorServer("ServerY (Worker) | Error occured while reading the request from parent process\n");
		else
			return;
	}
	// Making the child process busy
	busyCountY_sm[0]++;

	// Informing the user that request has received by the worker
	int size = get_matrix_size(request.pathToDataFile);

	sprintf(buffer,"%sWorker PID#%d is handling client PID#%d, matrix size %dx%d, pool %d/%d\n", 
			getTimeAndPID(),
			getpid(), 
			request.pid, 
			size, size, 
			busyCountY_sm[0], 
			pool_size);
	writeToFile_wFlag(path_to_log_file, buffer, "wop");

	// Finding the invertibility
	response.isInvertible = is_matrix_invertible(get_matrix_int(request.pathToDataFile));
	sleep(sleep_time);

	// Defining the client fifo path
	snprintf(path_to_client_fifo, CLIENT_FIFO_LENGTH, CLIENT_FIFO_TEMPLATE, (long)request.pid);

	// Adding result to global result
	result_sm->total += 1;
	if(response.isInvertible == INVERTIBLE)
		result_sm->invertible += 1;
	else
		result_sm->nonInvertible += 1;

	if(sigintFlag == SIGNAL)
		return;

	// Opening the client fifo
	clientFD = open(path_to_client_fifo, O_WRONLY);

	if(clientFD == -1)
		exitWithErrorServer("ServerY | Error occured while opening the client FIFO\n");

	// Sending the response
	if(write(clientFD, &response, sizeof(response_t)) != sizeof(response_t))
		exitWithErrorServer("ServerY | Error occured while sending the response.");

	// Close the client FIFO
	if(close(clientFD) == -1)
		exitWithErrorServer("ServerY | Error occured while closing the client FIFO\n");

	// Informing the user
	if(response.isInvertible == INVERTIBLE)
		sprintf(buffer, "%sWorker PID#%d is responding to client PID#%d: the matrix IS invertible\n", getTimeAndPID(), getpid(), request.pid);
	else
		sprintf(buffer, "%sWorker PID#%d is responding to client PID#%d: the matrix IS NOT invertible\n", getTimeAndPID(), getpid(), request.pid);
	writeToFile_wFlag(path_to_log_file, buffer, "wop");


	// Making the child process ready
	busyCountY_sm[0]--;
}

void childProcessZ() {
	// Telling to semaphore this child is ready for next request
	if(sem_post(sem_available_child) == -1)
		if(sigintFlag == SIGNAL)
			return; 
	// Waiting for a request to come
	if(sem_wait(sem_workerZ) == -1)
		if(sigintFlag == SIGNAL)
			return; 
	
	request_t queueRequest = removeFromQueue();

	// Making the child process busy
	busyCountZ_sm[0]++;
	
	// Informing the user that request has received by the worker
	int size = get_matrix_size(queueRequest.pathToDataFile);
	sprintf(buffer, "%sZ:Worker PID#%d is handling client PID#%d, matrix size %dx%d, pool %d/%d\n", 
			getTimeAndPID(),
			getpid(), 
			queueRequest.pid, 
			size, size, 
			busyCountZ_sm[0], 
			pool_size2);
	writeToFile_wFlag(path_to_log_file, buffer, "wop");

	// Finding the invertibility
	response.isInvertible = is_matrix_invertible(get_matrix_int(queueRequest.pathToDataFile));

	// Defining the client fifo path
	snprintf(path_to_client_fifo, CLIENT_FIFO_LENGTH, CLIENT_FIFO_TEMPLATE, (long)queueRequest.pid);
	sleep(sleep_time);
	// Making the child process ready
	busyCountZ_sm[0]--;

	// Adding result to global result
	resultZ_sm->total += 1;
	
	if(response.isInvertible == INVERTIBLE)
		resultZ_sm->invertible += 1;
	else
		resultZ_sm->nonInvertible += 1;
	// Adding result to shared memory result
	//memcpy(resultZ_sm, &resultZ, sizeof(result_t));

	if(sigintFlag == SIGNAL)
		return;

	// Opening the client fifo
	clientFD = open(path_to_client_fifo, O_WRONLY);

	if(clientFD == -1)
		exitWithErrorServer("ServerZ (Child) | Error occured while opening the client FIFO\n");

	// Sending the response
	if(write(clientFD, &response, sizeof(response_t)) != sizeof(response_t))
		exitWithErrorServer("ServerZ (Child) | Error occured while sending the response\n");

	// Close the client FIFO
	if(close(clientFD) == -1)
		exitWithErrorServer("ServerZ (Child) | Error occured while closing the client FIFO\n");


	// Informing the user
	if(response.isInvertible == INVERTIBLE)
		sprintf(buffer, "%sZ:Worker PID#%d is responding to client PID#%d: the matrix IS invertible\n", getTimeAndPID(), getpid(), queueRequest.pid);
	else
		sprintf(buffer, "%sZ:Worker PID#%d is responding to client PID#%d: the matrix IS NOT invertible\n", getTimeAndPID(), getpid(), queueRequest.pid);
	writeToFile_wFlag(path_to_log_file, buffer, "wop");

}


/* Initializing & executing serverZ */

void startServerZ() {
	sprintf(buffer, "%sInstantiated server Z\n", getTimeAndPID()); writeToFile_wFlag(path_to_log_file, buffer, "wop");
	switch (fork()) {
		// Error occured
		case -1:
			exitWithErrorServer("\nServerY | Error occured while executing the fork\n");
			break;
		// Child process
		case 0:
			serverZ = getpid();
			processZ();
			break;
		// Parent process
		default:
			if(sigintFlag == SIGNAL) 
				terminateServerY();
			break;
	}
}

void processZ() {
	// Informing user that serverZ is started
	sprintf(buffer, "%sServer Z (%s, t=%d, r=%d) started\n", getTimeAndPID(), path_to_log_file, sleep_time, pool_size2);
	writeToFile_wFlag(path_to_log_file, buffer, "wop");
	
	if(sigintFlag == SIGNAL) {
		terminateServerZ();
		return;
	}

	// Creating child processes
	int pid = fork();
	for(int child = 1; child < pool_size2; child++) {
		if(pid == -1) 
			exitWithErrorServer("\nServerZ | Error occured while executing the fork\n");
		else if(pid != 0) 
			pid = fork();
		else if(pid == 0)
			workersZ[child-1] = getpid();
	}
	// Reading requests and sending sending back responses
	for(;;) {
		if(pid != 0)
			parentProcessZ();
		else 
			childProcessZ();

		if(sigintFlag == SIGNAL)
			break;
	}

	if(pid != 0) 
		terminateServerZ();
}


// Signal Handling for Server 
void handleSIG(int signal) {
    switch (signal) {
        case SIGINT:
            sigintFlag = 1; 
            break;
        default:
            break;
    }
}


/* Terminating operations */

void terminateServerY() {
	char termStr[] = "SIGINT received, terminating Z and exiting server Y. ";
	
	waitpid(serverZ, 0, 0);
	waitpid(workerQ, 0, 0);
	for(int i=0; i<pool_size; i++)
		wait(NULL);
	sleep(1);
	
	sprintf(buffer, "%s%s Total requests handled: %d, %d invertible, %d not. %d requests were forwarded.\n", 
			getTimeAndPID(),
			termStr, 
			result_sm->total + resultZ_sm->total, 
			result_sm->invertible + resultZ_sm->invertible,
			result_sm->nonInvertible + resultZ_sm->nonInvertible,
			resultZ_sm->total);
	writeToFile_wFlag(path_to_log_file, buffer, "wop");
	// Closing file descriptors
	close(serverFD);
	close(clientFD);
	close(dummyFD);
	close(childCountFD);
	sem_close(sem_available_child);
	sem_close(sem_queue);
	sem_close(sem_workerZ);
	// Removing temp files
	unlink(path_to_client_fifo);
	unlink(path_to_server_fifo);
	sem_unlink(SEMAPHORE_PATH_AW);
	sem_unlink(SEMAPHORE_PATH_Q);
	sem_unlink(SEMAPHORE_PATH_Z);
	// Freeing allocated variables
	wait(NULL);
	exit(EXIT_SUCCESS);
}

void terminateServerZ() {
	char termStr[200] = "Z:SIGINT received, received, exiting server Z. ";

	sprintf(buffer, "%s%s Total requests handled %d, %d invertible, %d not.\n", 
			getTimeAndPID(),
			termStr, 
			resultZ_sm->total, 
			resultZ_sm->invertible,
			resultZ_sm->nonInvertible);
	writeToFile_wFlag(path_to_log_file, buffer, "wop");

	for(int i=0; i<pool_size2; i++)
		wait(NULL);

	// Closing file descriptors
	close(Zpfd[READ]);
	close(Zpfd[WRITE]);
}

void exitWithErrorServer(char* errMsg) {
    // Printing it out with stderr
	sprintf(buffer, "%s%s", getTimeAndPID(), errMsg);
	writeToFile_wFlag(path_to_log_file, buffer, "wop");
    // Terminating the program after error msg
    exit(EXIT_FAILURE);
}

char* getTimeAndPID() {
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


	sprintf(buffer, "%d/%d/%d | %d:%d:%d -> ", day, month, year, hour, min, sec);
	return buffer;
}


// Deletes serverY file after termination
void unlinkServer() {
	if(unlink(SERVERY_FILE) == -1)
        exitWithError("Error occured while removing the FIFO file.\n");
}


/* Queue Functions */

void startQueueWorker() {
	switch (fork()) {
		// Error occured
		case -1:
			exitWithErrorServer("\nServerY | Error occured while executing the fork\n");
			break;
		// Child process
		case 0:
			workerQ = getpid();
			queueWorker();
			break;
		// Parent process
		default:
			if(sigintFlag == SIGNAL) 
				terminateServerY();
			break;
	}
}

void queueWorker() {
	for(;;) {
		sem_wait(sem_available_child); if(sigintFlag == SIGNAL) break;
		sem_wait(sem_queue); if(sigintFlag == SIGNAL) break;
		
		// Telling one child process can answer this request
		sem_post(sem_workerZ);
		if(sigintFlag == SIGNAL) break;
	}
}

void addToQueue(request_t newRequest) {
    if(inQueue_sm[0] == QUEUE_LEN) {
        printf("Reached max size of the queue\n");
        return;
    }
    requestQueue_sm[inQueue_sm[0]] = newRequest;
    inQueue_sm[0]++;

	sem_post(sem_queue);
}

request_t removeFromQueue() {
    if(inQueue_sm[0] == 0) {
        request_t emptyRequest;
        emptyRequest.pid = -1;
        return emptyRequest;
    }

    request_t temp = requestQueue_sm[0];
    for(int i=0; i<inQueue_sm[0]-1; i++)
        requestQueue_sm[i] = requestQueue_sm[i+1];
    inQueue_sm[0]--;

    return temp;
}


// End //