#ifndef CLIENT_H
#define CLIENT_H
#include "utilities/define.h"
#include "utilities/file.h"
#include "data_structures/city.h"
#include "networking/request.h"



// Command line arguments
typedef struct cla_s {
    int port;
    char ipAddress[MAX_IP_LENGTH];
    char requestFile[MAX_REQFILE_LEN];
} cla_t; 

// Controls command line arguments and assigns them to their global variables
void ctrlcla(int argc, char* argv[]);
// Reads requestFile and stores every request inside requests object
void loadRequestsFromFile();
// Gets the date in form of "01-01-2000" and inserts day month and year values into cityTransaction object
void tokenizeDates(char* date1, char* date2);

// Creates a socket for client part and waits for server to connect
int createClientSocket();
// After getting the request count, this function is called to create that number of threads
// These threads will be responsible for sending request to the server and receiving response back
void createThreads();
// This function contains step5, step6 and step7 which must be executed within a thread function
// So instead of calling them separetaly, I made a general task function which executes these steps for each thread
void* threadTasks(void* arg);
// Makes all threads to wait until every thread is created and reached to this function
// Last coming thread broadcasts to everyone instead of waiting so code continues
void waitOthersOrBroadcast();
// Gets request_t object as a parameter and sends that request to the server using sockets
void sendRequestToServer(request_t* request, int connectFD);
// Reads back the response of the server and returns it after casting to int.
int receiveResponseFromServer(int connectFD);
// Joins to the all threads and after deallocating everything, it terminates the program
void joinThreadsAndTerminate();

// example output : "Client: I have loaded 10 requests and I’m creating 10 threads.\n"
void printDialogue1();
// example output : "Client-Thread-0: Thread-0 has been created"
void printDialogue2(int threadID);
// example output : "Client-Thread-5: I am requesting “/transactionCount TARLA 01-11-2021 30-11-2021 SAMSUN"
void printDialogue3(int threadID, char* requestStr);
// example output : "Client-Thread-5: The server’s response to “/transactionCount TARLA 01-11-2021 30-11-2021 SAMSUN” is 1"
void printDialogue4(int threadID, char* requestStr, int count, char* city);
// example output : "Client-Thread-5: Terminating"
void printDialogue5(int threadID);
// example output : "Client: All threads have terminated, goodbye."
void printDialogue6();

#endif 