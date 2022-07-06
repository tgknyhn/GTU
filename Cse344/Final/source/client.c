#define _GNU_SOURCE
#include "../header/data_structures/city.h"
#include "../header/utilities/mytime.h"
#include "../header/utilities/file.h"
#include "../header/client.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

cla_t args;                         // Command line arguments
request_t requests[MAX_REQUEST];    // Stores all request informations received from requestFile
int requestCount = 0;               // How many requests did this process read so far?

int createdThreadCount = 0;
pthread_t* threads;
pthread_mutex_t waitLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitCond = PTHREAD_COND_INITIALIZER;



/*  Steps
    -----
    1) Control command line arguments
    2) Load requests from requestsFile(or whatever name given with command line arguments)
    3) Create all threads
    4) Wait for all threads to be created
    5) Send requests to the server (also create socket and connection at this point)
    6) Receive responses from the server (close the socket after receiving response)
    7) Join all threads and terminate program
*/
int main(int argc, char *argv[])
{
    // Step 1
    ctrlcla(argc, argv);
    // Step 2
    loadRequestsFromFile();
    // Step 3
    createThreads(); // Steps 4, 5 and 6 is called inside threadTasks function
    // Step 7
    joinThreadsAndTerminate();

    return 0;
}

// Controls command line arguments and assigns them to their global variables
void ctrlcla(int argc, char* argv[]) {
    // controlling command line argument count
    if(argc != 7)
        exitWithError("Client: Unsufficient command line argument count. There must be 7 arguments.\n");
    // Controlling characters (-p and -t)
    if(strcmp(argv[1], "-r") != 0 || strcmp(argv[3], "-q") != 0 || strcmp(argv[5], "-s") != 0)
        exitWithError("Client: Invalid cla format. Valid example : \"./client -r ../requestFile -q 10000 -s 127.0.0.1\".\n");

    strcpy(args.requestFile, argv[2]);      // Request File
    args.port = atoi(argv[4]);              // Port number
    strcpy(args.ipAddress, argv[6]);        // IP Address

    // Validating argument values
    if(args.port < 2000)
        exitWithError("Client: Port value should be bigger than 2000 for safety. Terminating the server.\n");
}

// Reads requestFile and stores every request inside requests object
void loadRequestsFromFile() {
    // Open the request file
    FILE* fp = fopen(args.requestFile, "r");

    if(fp == NULL)
        exitWithError("Client: Error occured while opening the request file.\n");

    char * request = NULL;
    size_t length = 0;

    // Reading requests
    while(getline(&request, &length, fp) != -1) {
        if(strlen(request) < 2)
            continue;
        // Before using strtok, first insert it to requestStr
        strcpy(requests[requestCount].requestStr, request);
        // Splitting requests into variables 
        char * requestType = strtok(request, " ");
        char * realEstate  = strtok(NULL, " ");
        char * date1       = strtok(NULL, " ");
        char * date2       = strtok(NULL, " ");
        char * city        = strtok(NULL, " ");

        // Removing '\n' character
        if(city == NULL && date2[strlen(date2)-1] == '\n')
            date2[strlen(date2)-1] = '\0';
        else if(city[strlen(city)-1] == '\n')
            city[strlen(city)-1] = '\0';

        // Inserting request into the request_t struct object
        strcpy(requests[requestCount].requestType, requestType);
        strcpy(requests[requestCount].realEstate, realEstate);
        tokenizeDates(date1, date2);
        if(city != NULL) strcpy(requests[requestCount].city, city);
        requests[requestCount].id = requestCount;

        // Incrementing the request count by 1
        requestCount++;
    }
    // Printing out request count, etc.
    printDialogue1();
    // Deallocating the request char which is used to store lines
    free(request);
    // Closing the file
    fclose(fp);
} 

// Gets the date in form of "01-01-2000" and inserts day month and year values into cityTransaction object
void tokenizeDates(char* date1, char* date2) {
    int day1   = atoi(strtok(date1, "-"));
    int month1 = atoi(strtok(NULL, "-"));
    int year1  = atoi(strtok(NULL, "-"));

    int day2   = atoi(strtok(date2, "-"));
    int month2 = atoi(strtok(NULL, "-"));
    int year2  = atoi(strtok(NULL, "-"));

    if(day1 < 1 || day1 > 30 || day2 < 1 || day2 > 30)
        exitWithError("Client: Day value is wrong. Days are between 1-30. Terminating client process.\n");
    if(month1 < 1 || month1 > 12 || month2 < 1 || month2 > 12)
        exitWithError("Client: Month value is wrong. Months are between 1-12. Terminating client process.\n");
    if(year1 < 2000 || year2 < 2000)
        exitWithError("Client: Year value is wrong. Years start with 2000. Terminating client process.\n");
    
    requests[requestCount].dayStart   = day1;
    requests[requestCount].monthStart = month1;
    requests[requestCount].yearStart  = year1;
    
    requests[requestCount].dayEnd   = day2;
    requests[requestCount].monthEnd = month2;
    requests[requestCount].yearEnd  = year2;
}

// Creates a socket for client part and waits for server to connect
int createClientSocket() {
    int socketFD;
    struct sockaddr_in serverAddr;
    // Setting serverAddr attributes
    serverAddr.sin_family      = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(args.ipAddress); 
    serverAddr.sin_port        = htons(args.port);

    // 1) Creating socket and getting the file descriptor
    if((socketFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exitWithError("Client: Error occured while getting socketFD.\n");
    
    // 2) Connecting client to the ipc and waiting for server
    if(connect(socketFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        exitWithError("Client: Error occured while connecting client socket.\n");

    return socketFD;
}

// After getting the request count, this function is called to create that number of threads
// These threads will be responsible for sending request to the server and receiving response back
void createThreads() {
    // Allocate space for threads
    threads = (pthread_t*)malloc(sizeof(pthread_t) * requestCount);
    // Creating threads
    for(int i=0; i<requestCount; i++) {
        // Create
        pthread_create(&threads[i], NULL, threadTasks, (void*)(&requests[i]));
        // Print out
        printDialogue2(i);
    }
}

// This function contains step5, step6 and step7 which must be executed within a thread function
// So instead of calling them separetaly, I made a general task function which executes these steps for each thread
void* threadTasks(void* arg) {
    // First cast arg to index_t
    request_t* request = (request_t*)arg;
    // Wait other threads before sending request
    waitOthersOrBroadcast();
    // Step 4
    int connectFD = createClientSocket();
    // Printing out the request
    printDialogue3(request->id, request->requestStr);
    // Step 5
    sendRequestToServer(request, connectFD);
    // Step 6
    int count = receiveResponseFromServer(connectFD);
    // Printing out the response
    printDialogue4(request->id, request->requestStr, count, request->city);
    // Finish the thread
    return NULL;
}

// Makes all threads to wait until every thread is created and reached to this function
// Last coming thread broadcasts to everyone instead of waiting so code continues
void waitOthersOrBroadcast() {
    // Lock the mutex to enter critical region
    if(pthread_mutex_lock(&waitLock) != 0)
        exitWithError("Error occured while executing pthread_mutex_lock.\n");
    // Increment the created thread count
    createdThreadCount++;     // Wait until every thread is created
    while(createdThreadCount < requestCount)
        pthread_cond_wait(&waitCond, &waitLock);
    // broadcast since this is the last thread
    pthread_cond_broadcast(&waitCond);
    // Release the lock since we finished our job in critical region
    pthread_mutex_unlock(&waitLock);
}

// Gets request_t object as a parameter and sends that request to the server using sockets
void sendRequestToServer(request_t* request, int connectFD) {
    // Getting length of the request
    size_t length = sizeof(*request);
    // Sending the request
    int size = write(connectFD, request, length);
    // Error check
    if(size != length)
        exitWithError("Client: Error occured while sending the request.\n");
}

// Reads back the response of the server and prints it out.
int receiveResponseFromServer(int connectFD) {
    char buffer[10] = {0};
    // Reading the server's response
    read(connectFD, buffer, 10);
    // Closing the socket
    close(connectFD);
    // Returning the response
    return atoi(buffer);
}

// Joins to the all threads and after deallocating everything, it terminates the program
void joinThreadsAndTerminate() {
    // Joining to the threads
    for(int i=0; i<requestCount; i++) {
        pthread_join(threads[i], NULL);
        printDialogue5(i);
    }
    // Deallocating the heap allocated variables and closing sockets since we no longer need them
    free(threads);
    // Print last dialogue
    printDialogue6();
}

// example output : "Client: I have loaded 10 requests and I’m creating 10 threads.\n"
void printDialogue1() {
    char timeSTR[100]; getCurrentTime(timeSTR);
    printf("%s Client: I have loaded %d requests and I''m creating %d threads.\n", timeSTR, requestCount, requestCount);
}

// example output : "Client-Thread-0: Thread-0 has been created"
void printDialogue2(int threadID) {
    char timeSTR[100]; getCurrentTime(timeSTR);
    printf("%s Client-Thread-%d: Thread-%d has been created\n", timeSTR, threadID, threadID);
}

// example output : "Client-Thread-5: I am requesting “/transactionCount TARLA 01-11-2021 30-11-2021 SAMSUN"
void printDialogue3(int threadID, char* requestStr) {
    char timeSTR[100]; getCurrentTime(timeSTR);
    requestStr[strlen(requestStr)-1] = '\0';
    printf("%s Client-Thread-%d: I am requesting \"%s\"\n", timeSTR, threadID, requestStr);
}

// example output : "Client-Thread-5: The server’s response to “/transactionCount TARLA 01-11-2021 30-11-2021 SAMSUN” is 1"
void printDialogue4(int threadID, char* requestStr, int count, char* city) {
    char timeSTR[100]; getCurrentTime(timeSTR);
    if(count == -1) 
        printf("%s Client-Thread-%d: The server sent an error. There is no city named %s.\n", timeSTR, threadID, city);
    else
        printf("%s Client-Thread-%d: The server's response to \"%s\" is %d\n", timeSTR, threadID, requestStr, count);

}

// example output : "Client-Thread-5: Terminating"
void printDialogue5(int threadID) {
    char timeSTR[100]; getCurrentTime(timeSTR);
    printf("%s Client-Thread-%d: Terminating\n", timeSTR, threadID);
}

// example output : "Client: All threads have terminated, goodbye."
void printDialogue6() {
    char timeSTR[100]; getCurrentTime(timeSTR);
    printf("%s Client: All threads have terminated, goodbye.\n", timeSTR);
}


/* END */