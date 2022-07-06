#include "../header/main.h"
#include "../header/file.h"
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h> 
#include <time.h>

// Command Line Arguments
char path_to_input_file[PATH_LEN];
int  C;
int  N;

char timeBuffer[MAX_BUFFER_SIZE];
int sigintFlag = NOSIGNAL;
int semaphoresID;
key_t semKey;
semun_t semAttr;

pthread_t  supplierThread;
pthread_t* consumerThreads;
pthread_attr_t supplierAttr;

int main(int argc, char *argv[])
{
    // Check command line arguments
    ctrlcla(argc, argv);
    // Initializing signal handler
    initializeSignalHandler();
    // Initialize system V semaphoresID before creating threads
    initializeSysmtemVsemaphores();
    // Start supplier thread
    startSupplier();
    // Start consumer threads
    startConsumers();
    // Wait for the consumer threads to terminate
    joinThreads();

    return 0;
}

void ctrlcla(int argc, char* argv[]) {
	if(argc != 7)
		exitWithError("Wrong argument count. Valid: 7 arguments.\n");
	
    if(strcmp(argv[1], "-C") == 0 && strcmp(argv[3], "-N") == 0 && strcmp(argv[5], "-F") == 0) {
        C = atoi(argv[2]);
        N = atoi(argv[4]);
		strcpy(path_to_input_file, argv[6]);
    }
	else
		exitWithError("Invalid CLA. It should be something as: \"./hw4 -C 10 -N 5 -F inputFilePath\"\n");

    if(C <= 4)
        exitWithError("C count must be bigger than 4.\n");
    if(N <= 1)
        exitWithError("N count must be bigger than 1.\n");
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
    if(sigaction(SIGINT, &sigActionStruct, NULL) == -1) 
        exitWithError("Error occured at signal initialization.\n");
}

void initializeSysmtemVsemaphores() {
    semKey = ftok(SEM_PATH, 'a');
    if(semKey == -1)
        exitWithError("Error occured at ftok");
    
    semaphoresID = semget(semKey, 2, 0666 | IPC_CREAT);
    if(semaphoresID == -1) {
        exitWithError("Error occured at semget");
    }

	semAttr.val = 0;    
    if(semctl(semaphoresID, 0, SETVAL, semAttr) == -1) 
        exitWithError("Error occured at semctl");

    if(semctl(semaphoresID, 1, SETVAL, semAttr) == -1) 
        exitWithError("Error occured at semctl");
}

void startSupplier() {
    // Initialize the supplier attribute
    if(pthread_attr_init(&supplierAttr) != 0)
        exitWithError("Error occured at pthread_attr_init");

    // Setting pthread as detached
    if(pthread_attr_setdetachstate(&supplierAttr, PTHREAD_CREATE_DETACHED) != 0)
        exitWithError("Error occured at pthread_attr_setdetachstate");
    
    // Create supplier thread
    if(pthread_create(&supplierThread, &supplierAttr, supplier, NULL) != 0)
        exitWithError("Error occured at pthread_create");

    // Destroy supplier attribute since no longer needed
    if(pthread_attr_destroy(&supplierAttr) != 0)
        exitWithError("Error occured at pthread_attr_destroy");
}

void startConsumers() {
    // Allocate space for consumer threads
    consumerThreads = (pthread_t*)malloc(sizeof(pthread_t) * C);
    // Create consumer threads
    for(int i=0; i<C; i++) {
        int* id = (int*)malloc(sizeof(int));
        *id = i; 
        pthread_create(&consumerThreads[i], NULL, consumer, (void*)id);
    }
}

void* supplier(void* arg) {
    struct sembuf pushSemaphore;
    // Allocate space for file buffer
    char * buffer = (char*)malloc(sizeof(char));
    // Open the file and get file descriptor
    int readFD = openFile(path_to_input_file, "r");
    // Read first character
    int readSize = readFromFileFD(readFD, buffer, 1);
    // Read rest
    while(readSize != 0) {
        if(sigintFlag == SIGNAL)
            break;
        // Print to the console before delivery
        printBeforeDelivery(buffer[0]);
        // Control buffer character and post semaphore according to it
        if(buffer[0] == '1') {
            pushSemaphore.sem_num = 0;
            pushSemaphore.sem_op = 1;
            pushSemaphore.sem_flg = 0;
            if(semop(semaphoresID, &pushSemaphore, 1) == -1)
                exitWithError("Error occured at semop (post character 1)");
        }
        else if(buffer[0] == '2') {
            pushSemaphore.sem_num = 1;
            pushSemaphore.sem_op = 1;
            pushSemaphore.sem_flg = 0;
            if(semop(semaphoresID, &pushSemaphore, 1) == -1)
                exitWithError("Error occured at semop (post character 2)");
        }
        // Print to the console after delivery
        printAfterDelivery(buffer[0]);
        // Read next character
        readSize = readFromFileFD(readFD, buffer, 1);
    }
    closeFile(readFD);
    printSupplierExit();
    // Deallocate the file buffer
    free(buffer);
    return NULL;
}

void* consumer(void* arg) {
    struct sembuf doubleSem[2];
    // Consumer id
    int id = *((int*)arg);

    for(int i=0; i<N; i++) {
        if(sigintFlag == SIGNAL)
            break;
        // Correct semaphore attributes
        doubleSem[0].sem_num = 0;
        doubleSem[0].sem_op = -1;
        doubleSem[0].sem_flg = 0;

        doubleSem[1].sem_num = 1;
        doubleSem[1].sem_op = -1;
        doubleSem[1].sem_flg = 0;

        // Printing info to the console before consuming
        printBeforeConsuming(id, i);
        // Waiting for supplier
        if(semop(semaphoresID, doubleSem, 2) == -1) 
            exitWithError("Error occured at semop (wait consumer)");
        // Printing info to the console before consuming
        printAfterConsuming(id, i);
    }
    free(arg);
    printConsumerExit(id);
    return NULL;
}

void joinThreads() {
    // Joining consumer threads
    for(int i=0; i<C; i++)
        pthread_join(consumerThreads[i], NULL);

    if(semctl(semaphoresID, 0, IPC_RMID)==-1)
        exitWithError("Error occured at semctl (removing semaphoresID)");

    // Deallocate consumerThreads variable
    free(consumerThreads);
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

void printBeforeDelivery(char buffer) {
    char printBuffer[MAX_BUFFER_SIZE];
    
    int oneCount = semctl(semaphoresID, 0, GETVAL, semAttr);
    int twoCount = semctl(semaphoresID, 1, GETVAL, semAttr);

    sprintf(printBuffer, "%sSupplier: read from input '%c'. Current amounts: %d x '1', %d x '2'.\n",
            getTimeBuffer(),
            buffer,
            oneCount,
            twoCount);

    writeToFileFD(STDOUT_FILENO, printBuffer, strlen(printBuffer));
}

void printAfterDelivery(char buffer) {
    char printBuffer[MAX_BUFFER_SIZE];
    
    int oneCount = semctl(semaphoresID, 0, GETVAL, semAttr);
    int twoCount = semctl(semaphoresID, 1, GETVAL, semAttr);

    sprintf(printBuffer, "%sSupplier: delivered a '%c'. Post-delivery amounts: %d x '1', %d x '2'.\n",
            getTimeBuffer(),
            buffer,
            oneCount,
            twoCount);

    writeToFileFD(STDOUT_FILENO, printBuffer, strlen(printBuffer));
}

void printBeforeConsuming(int id, int iteration) {
    char printBuffer[MAX_BUFFER_SIZE];
    
    int oneCount = semctl(semaphoresID, 0, GETVAL, semAttr);
    int twoCount = semctl(semaphoresID, 1, GETVAL, semAttr);

    sprintf(printBuffer, "%sConsumer-%d at iteration %d (waiting). Current amounts: %d x '1', %d x '2'.\n",
            getTimeBuffer(),
            id, iteration,
            oneCount,
            twoCount);

    writeToFileFD(STDOUT_FILENO, printBuffer, strlen(printBuffer));
}

void printAfterConsuming(int id, int iteration) {
    char printBuffer[MAX_BUFFER_SIZE];
    
    int oneCount = semctl(semaphoresID, 0, GETVAL, semAttr);
    int twoCount = semctl(semaphoresID, 1, GETVAL, semAttr);

    sprintf(printBuffer, "%sConsumer-%d at iteration %d (consumed). Post-consumption amounts: %d x '1', %d x '2'.\n",
            getTimeBuffer(),
            id, iteration,
            oneCount,
            twoCount);

    writeToFileFD(STDOUT_FILENO, printBuffer, strlen(printBuffer));
}

void printSupplierExit() {
    char printBuffer[MAX_BUFFER_SIZE];
    
    sprintf(printBuffer, "%sThe supplier has left.\n", getTimeBuffer());

    writeToFileFD(STDOUT_FILENO, printBuffer, strlen(printBuffer));
}

void printConsumerExit(int id) {
    char printBuffer[MAX_BUFFER_SIZE];
    
    sprintf(printBuffer, "%sThe consumer-%d has left.\n", getTimeBuffer(), id);

    writeToFileFD(STDOUT_FILENO, printBuffer, strlen(printBuffer));
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

/* End */