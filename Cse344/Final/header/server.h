#ifndef SERVER_H
#define SERVER_H
#include "utilities/define.h"
#include "utilities/file.h"
#include "data_structures/city.h"
#include "networking/request.h"

// Command line arguments
typedef struct cla_s {
    int port;
    int numberOfThreads;
} cla_t; 


// Controls command line arguments and assigns them to their global variables
void ctrlcla(int argc, char* argv[]);
// Returns how many city directory are there inside dataset
int getCitySize(char* directory);
// Reads all subdirectoires of current directory and assigns them into cityNames variable
void setCityNames(char* directory);
// Gets a city name as parameter and returns it's index in the cityNames array
// Returns -1 when cityName is not in the cityNames array
int getCityIndex(char* cityName);
// How many threads will be created is decided with command line arguments
// This value is stored in args.numberOfThreads
// This function creates that amount of threads to handle incoming connections
// From both servant and client
void createThreads();
// Creates a socket for server
void createServerSocket();
// Creates a client socket with given port and id and returns file descriptor 
int createClientSocket(int port, char ip[MAX_IP_LENGTH]);


// Blocks the process until a request arrives
void waitForRequest();
// Reads requests coming from both servent and client
// Handles these requests by calling handleServantRequest and handleClientRequest functions according to readSize
// Then returns the response back to servant or client
void* handleRequest(void* p_connectFD);
// Gets servant's request as a parameter(as a servantInfo_t pointer)
// And stores all necessary information about servant for later use
void handleServantRequest(servantInfo_t* servantRequest);
// Gets request_t object as a parameter(which came from client)
// And after finding relevant servants, it sends them a request
// Collects all data and returns the final result back to client
void handleClientRequest(request_t* request, int socketFD);
// Sends request to servant to get information about client request and returns the result
int sendRequestToServant(servantInfo_t* servantInfo, request_t* request);
// Joins to the all threads and after deallocating everything, it terminates the program
void joinThreadsAndTerminate();

// Output example : "Servant 6672 present at port 16001 handling cities ADANA-ANKARA"
void printDialogue1(servantInfo_t* info);
// Output example : "Request arrived “transactionCount TARLA 01-01-2000 10-01-2021 ISTANBUL"
// Output example : "Contacting servant 6675"
// Output example : "Contacting ALL servants"
// Output example : "Response received: 11, forwarded to client"
void printDialogue2(char* requestStr, int servantPID, int count, char* city);
// Output example : "SIGINT has been received. I handled a total of 30 requests. Goodbye."
void printDialogue3(int count);


// Initializes the signal handler using sigaction
// Also blocks every other signal except SIGINT using sigprocmask
void initializeSignalHandler();
// It only changes the value of given signal from 0 to 1
void handleSIG(int signal);


#endif