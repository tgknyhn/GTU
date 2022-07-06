#ifndef SERVERY_H
#define SERVERY_H

#define SEMAPHORE_PATH_Z "/SemWorkerZ"
#define SEMAPHORE_PATH_Q "/SemQueue"
#define SEMAPHORE_PATH_AW "/SemAvailableChild"


// Pipe Indexes
#define READ 0
#define WRITE 1
// bool for SIGINT signal
#define SIGNAL 1
#define NO_SIGNAL 0
// server file path which used to prevent double instance
#define SERVERY_FILE "serverY.txt"
// How many request can be hold in queue
#define QUEUE_LEN 1000
#define BUFFER_SIZE 300

// Called to prevent executing more than one serverY process
void preventDoubleInstance(char* temp_path);
// Constrols command line arguments
void control_cla(int argc, char* argv[]);
// Initializing part
void initializeQueue();
void initializeSemaphores();
void initializeDummyFIFO();
void initializeServerFIFO();
void initializeResultStruct();
void initializeSignalHandler();
void initializeChildCountFile();
// Creates pool_size children processes 
int createChildrenProcesses();
// Parent processes which wait for requests
void parentProcessY();
void parentProcessZ();
// Child(worker) processes which send the responde
void childProcessY();
void childProcessZ();
// Initializing & executing serverZ
void startServerZ();
void processZ();
// Busy count handlers
void incrementBusyChildCount(char* text_path);
void decrementBusyChildCount(char* text_path);
int  getBusyCount(char* file_path);
// Signal Handling for Server 
void handleSIG(int signal);
// Terminating operations
void terminateServerY();
void terminateServerZ();
void exitWithErrorServer(char* errMsg);
// Prints out time and process id to log file
char* getTimeAndPID();
// Deletes serverY file after termination
void unlinkServer();
// Queue functions
void startQueueWorker();
void queueWorker();
void addToQueue(request_t newRequest);
request_t removeFromQueue();

#endif