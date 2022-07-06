#include "../header/data_structures/queue.h"
#include "../header/utilities/mytime.h"
#include "../header/server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <dirent.h>

cla_t         args;                          // Command line arguments
servantInfo_t servants[MAX_SERVANT_COUNT];   // Servants' informations which connacted with the server
int           currServantCount = 0;          // How many servant connected so far
char**        cityNames = NULL;              // All 81 cities and their names 
int           totalCityCount;

struct sockaddr_in address;
int addressLength = sizeof(address);
int serverFD;

pthread_t* threads;
pthread_mutex_t waitLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitCond = PTHREAD_COND_INITIALIZER;

int sigintFlag = NOSIGNAL;
int sigintThreadFlag = NOSIGNAL;

/*  Steps
    -----
    1) Control command line arguments
    2) Initialize signal handler
    3) Create server socket
    4) Create threads 
    5) Wait for requests and send them to threads. They handle the request and send response back
    6) Join threads after SIGINT received and terminate
*/
int main(int argc, char *argv[])
{
    // Step 1
    ctrlcla(argc, argv);
    // Step 2
    initializeSignalHandler();
    // Step 3
    createServerSocket();
    // Step 4
    createThreads();
    // Step 5
    waitForRequest();
    // Step 6
    joinThreadsAndTerminate();

    return 0;
}

// Controls command line arguments and assigns them to their global variables
void ctrlcla(int argc, char* argv[]) {
    // Controlling argument count
    if(argc != 5)
        exitWithError("Server: Unsufficient command line argument count. There must be 5 arguments.\n");
    // Controlling characters (-p and -t)
    if(strcmp(argv[1], "-p") != 0 || strcmp(argv[3], "-t") != 0)
        exitWithError("Server: Invalid cla format. Valid example : \"./server -p 3000 -t 11\".\n");

    // Inserting command line arguments into global variables
    args.port            = atoi(argv[2]);
    args.numberOfThreads = atoi(argv[4]);

    // Validating argument values
    if(args.port < 2000)
        exitWithError("Server: Port value should be bigger than 2000 for safety. Terminating the server.\n");
    if(args.numberOfThreads < 5)
        exitWithError("Server: Number of threads can't be lower than 5. Terminating the server.\n");

}

// Returns how many city directory are there inside dataset
int getCitySize(char* directory) {
    // Declaring variables
    struct dirent **d_cityNames;
    int numberOfCities;
    // I removed 2 because in these city names both '.' and '..' are included
    numberOfCities = scandir(directory, &d_cityNames, NULL, alphasort) - 2;
    // Deallocate the d_cityNames
    for(int i=0; i<numberOfCities+2; i++)
        free(d_cityNames[i]);
    free(d_cityNames);

    return numberOfCities;
}

// Reads all subdirectoires of current directory and assigns them into cityNames variable
void setCityNames(char* directory) {
    // First allocate space
    cityNames = (char**)malloc(sizeof(char*) * totalCityCount);  
    for(int i=0; i<totalCityCount; i++)
        cityNames[i] = (char*)malloc(sizeof(char) * MAX_CITY_LEN);    
    // Declaring variables
    struct dirent **d_cityNames;
    int numberOfCities;
    // Reading the dataset directory and getting the list of city names into d_cityNames
    // I removed 2 because in these city names both '.' and '..' are included
    numberOfCities = scandir(directory, &d_cityNames, NULL, alphasort) - 2;
    if(numberOfCities == -3)
        exitWithError("Error occured while calling scandir.\n");
    // Copying city names into global variable
    for(int i=0; i<numberOfCities; i++)
        strcpy(cityNames[i], d_cityNames[i+2]->d_name);
    // Deallocate the d_cityNames
    for(int i=0; i<numberOfCities+2; i++)
        free(d_cityNames[i]);
    free(d_cityNames);
}

// Gets a city name as parameter and returns it's index in the cityNames array
// Returns -1 when cityName is not in the cityNames array
int getCityIndex(char* cityName) {
    if(cityName == NULL)
        return -2;

    for(int i=0; i<TOTAL_CITY_COUNT; i++) 
        if(strcmp(cityName, cityNames[i]) == 0)
            return i;
    
    return -2;
}

// How many threads will be created is decided with command line arguments
// This value is stored in args.numberOfThreads
// This function creates that amount of threads to handle incoming connections
// From both servant and client
void createThreads() {
    // Allocate space for threads
    threads = (pthread_t*)malloc(sizeof(pthread_t) * args.numberOfThreads);
    // Creating threads
    for(int i=0; i<args.numberOfThreads; i++) 
        pthread_create(&threads[i], NULL, handleRequest, NULL);
}

// Creates a socket for server
void createServerSocket() {
    // Setting serverAddr attributes
	address.sin_family      = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port        = htons(args.port);

    // 1) Creating socket and getting the file descriptor
	if((serverFD = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        exitWithError("Server: Error occurred while calling socket function.\n");
        
    // 2) Make server address rebindable
    int opt = 1;
    if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        exitWithError("Server: Error occured while calling setsockopt.\n");

    // 3) Associating the server address with the socket using file descriptor
	if(bind(serverFD, (struct sockaddr*)&address, sizeof(address)) < 0)
        exitWithError("Server: Error occured while binding the socket.\n");

    
    // 4) Making socket go silent and wait for a connection 
	if(listen(serverFD, 20) < 0) 
        exitWithError("Server: Error occured while listening the socket.\n");
}

// Creates a client socket with given port and id and returns file descriptor 
int createClientSocket(int port, char ip[MAX_IP_LENGTH]) {
    int clientFD;
    struct sockaddr_in clientAddr;

    // Setting serverAddr attributes
    clientAddr.sin_family      = AF_INET;
    clientAddr.sin_addr.s_addr = inet_addr(ip); 
    clientAddr.sin_port        = htons(port);

    // 1) Creating socket and getting the file descriptor
    if((clientFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exitWithError("Server: Error occured while getting socketFD.\n");
    

    // 2) Connecting client to the ipc and waiting for server
    if(connect(clientFD, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) == -1)
        exitWithError("Server: Error occured while connecting client socket.\n");
    
    return clientFD;
}

// Blocks the process until a request arrives
void waitForRequest() {
    int connectFD;
    for(;;) {
        // Getting new socket and accepting requests from clients
        if((connectFD = accept(serverFD, (struct sockaddr*)&address, (socklen_t*)&addressLength)) < 0) {
            if(sigintFlag == SIGNAL)
                break;
            exitWithError("Server: Error occurred while accepting the request.\n");
        }
        // If code reaches here, it means a request is arrived.
        // So by using enqueue function from queue library I wrote, we can add this request to the pool of requests.
        // But the important thing is, if we don't protect this enqueue operation with mutexes, then race candition might occur
        // After protecting the critical region and achieving mutual exclusion, another thing we need to consider is busy waiting
        // To prevent busy waiting in function handleRequest, which executed by threads, we need to use a condition variable 
        // Together with a mutex. Well this is what monitor is actually. So by sending signals after getting a request
        // We can prevent busy waiting. Until signal arrives, all threads will wait for a request signal
        pthread_mutex_lock(&waitLock);
        enqueue(connectFD);
        pthread_cond_signal(&waitCond);
        pthread_mutex_unlock(&waitLock);
    }
    // If main thread reaches here, that means it received SIGINT
    // So broadcast to all threads to terminate them either
    pthread_cond_broadcast(&waitCond);
    // We have every servantPID so we send kill signal to them too
    for(int i=0; i<currServantCount; i++)
        kill(servants[i].servantPID, SIGINT);
}

// Reads requests coming from both servent and client
// Handles these requests by calling handleServantRequest and handleClientRequest functions according to readSize
// Then returns the response back to servant or client
void* handleRequest(void* arg) {
    // Allocating a void pointer buffer to read request
    void* buffer = (void*)calloc(sizeof(void), 1024);
    int handlerFD = -1;
    int * count = (int*)malloc(sizeof(int));
    *count = 0;
    for(;;) {
        // The same thing we did in waitForRequest function was essential.
        // But to make it work, achieve mutual exclusion, same thing must be done here
        // So we will use mutexes while calling the dequeue function
        pthread_mutex_lock(&waitLock);
        // Before calling dequeue function, first we need to call cond_wait to prevent busy waiting
        // But if we call cond_wait without checking dequeue return value, then our handler function
        // Might wait even though there is a request to handle. To prevent that, we need to check
        // if there is a request at the moment or not, then we call cond_wait
        if((handlerFD = dequeue()) == -1) { 
            // There is nothing in the queue, so we wait
            pthread_cond_wait(&waitCond, &waitLock);
            // Signal is arrived and now we can get request and handle it
            handlerFD = dequeue();
        }
        pthread_mutex_unlock(&waitLock);
        // Reading the request into void buffer
        int readSize = read(handlerFD, buffer, 1024);
        // Before handling request check for SIGINT
        if(sigintFlag == SIGNAL)
            break;
        // Checking the size and handling according to that read size
        // If readen byte amount is between these numbers, then this mean request came from a servant
        if(readSize >= 30 && readSize <= 200) 
            handleServantRequest((servantInfo_t*)buffer);
        else if(readSize == 380){
            handleClientRequest((request_t*)buffer, handlerFD);
            *count = *count + 1;
        }
        // Closing the fd
        close(handlerFD);
    }
    // Deallocate the buffer
    free(buffer);
    // Closing the fd
    close(handlerFD);
    // return NULL for now
    return (void*)count;
}

// Gets servant's request as a parameter(as a char pointer)
// And stores all necessary information about servant for later use
void handleServantRequest(servantInfo_t* servantRequest) {
    pthread_mutex_lock(&waitLock);
    // Adding servantRequest to servants array
    servants[currServantCount].start      = servantRequest->start;
    servants[currServantCount].end        = servantRequest->end;
    servants[currServantCount].port       = servantRequest->port;
    servants[currServantCount].servantPID = servantRequest->servantPID;
    strcpy(servants[currServantCount].ipAddress, servantRequest->ipAddress);
    // Inform the user about this servant
    printDialogue1(servantRequest);
    // Incrementing the servant count by 1
    currServantCount++;
    pthread_mutex_unlock(&waitLock);
}

// Gets request_t object as a parameter(which came from client)
// And after finding relevant servants, it sends them a request
// Collects all data and returns the final result back to client
void handleClientRequest(request_t* request, int socketFD) {
    // Look if city name is present or not
    int cityExist = (strlen(request->city) == 0) ? 0 : 1;
    // Transaction count
    int count = 0;
    int servantIndex = 0;

    if(cityExist) {
        int cityIndex = getCityIndex(request->city) + 1;
        if(cityIndex == -1)
            count = -1;
        if(count != -1) {
            for(int i=0; i<currServantCount; i++) {
                if(servants[i].start <= cityIndex && servants[i].end >= cityIndex) {
                    servantIndex = i;
                    count = sendRequestToServant(&servants[i], request);
                }
            }
        }
    }
    else {
        for(int i=0; i<currServantCount; i++) 
            count += sendRequestToServant(&servants[i], request);
    }
    // First inform the user 
    printDialogue2(request->requestStr, servants[servantIndex].servantPID, count, request->city);
    // After getting the count, send the result back to client
    char buffer[10] = {0};
    sprintf(buffer, "%d", count);
    write(socketFD, buffer, strlen(buffer));
}

// Sends request to servant to get information about client request and returns the result
int sendRequestToServant(servantInfo_t* servantInfo, request_t* request) {
    char buffer[10] = {0};
    // First getting the socket for servant connection    printf("%s\n", request->requestStr);
    int toServantFD = createClientSocket(servantInfo->port, servantInfo->ipAddress);
    // If request is null that means we want to send a SIGINT signal
    // We do it by sending 1 byte
    if(request == NULL) {
        char empty[] = {'0'};
        write(toServantFD, (void*)&empty, 1);
        return 0;
    }
    write(toServantFD, (void*)request, sizeof(*request));
    // After sending, read back the response of servant
    read(toServantFD, buffer, 10);
    // Closing the servantFD
    close(toServantFD);
    // Return the count
    return atoi(buffer);
}

// Joins to the all threads and after deallocating everything, it terminates the program
void joinThreadsAndTerminate() {
    int * p_count;
    int count = 0;
    // Joining to the threads
    for(int i=0; i<args.numberOfThreads; i++) {
        pthread_join(threads[i], (void*)&p_count);
        count += *p_count;
        free(p_count);
    }
    // Inform the user that program terminates
    printDialogue3(count);
    // Deallocating the heap allocated variables and closing sockets since we no longer need them
    free(threads);
    for(int i=0; i<totalCityCount; i++)
        free(cityNames[i]);
    free(cityNames);
}

// Output example : "Servant 6672 present at port 16001 handling cities ADANA-ANKARA"
void printDialogue1(servantInfo_t* info) {
    // First set the city names if it is empty
    if(cityNames == NULL) {
        totalCityCount = getCitySize(info->directory);
        setCityNames(info->directory);
    }
    char timeSTR[100]; getCurrentTime(timeSTR);
    // Printing first city only
    printf("%s Servant %d present at port %d handling cities %s", 
            timeSTR,
            info->servantPID, 
            info->port, 
            cityNames[info->start-1]);
    // Printing rest of it
    for(int i=info->start; i<info->end; i++)
        printf("-%s", cityNames[i]);
    printf("\n");
}

// Output example : "Request arrived “transactionCount TARLA 01-01-2000 10-01-2021 ISTANBUL"
// Output example : "Contacting servant 6675"
// Output example : "Contacting ALL servants"
// Output example : "Response received: 11, forwarded to client"
void printDialogue2(char* requestStr, int servantPID, int count, char* city) {
    char timeSTR[100]; getCurrentTime(timeSTR);
    // Locking
    pthread_mutex_lock(&waitLock);
    // Print request
    printf("%s Request arrived \"%s\"\n", timeSTR, requestStr);
    // Print who to contact
    if(count == -1) 
        printf("%s There is no servant responsible for city: \"%s\". Sending an error to the client\n", timeSTR, city);
    else if(servantPID == -1) 
        printf("%s Contacting ALL servants\n", timeSTR);
    else
        printf("%s Contacting servant %d\n", timeSTR, servantPID);
    
    // Finally print the result
    if(count != -1)
        printf("%s Response received: %d, forwarded to client\n", timeSTR, count);
    // Unlocking
    pthread_mutex_unlock(&waitLock);
}

// Output example : "SIGINT has been received. I handled a total of 30 requests. Goodbye."
void printDialogue3(int count) {
    char timeSTR[100]; getCurrentTime(timeSTR);
    printf("\n%s SIGINT has been received. I handled a total of %d requests. Goodbye.\n", timeSTR, count);
}

// Initializes the signal handler using sigaction
// Also blocks every other signal except SIGINT using sigprocmask
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

// It only changes the value of given signal from 0 to 1
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