#ifndef SERVANT_H
#define SERVANT_H
#include <unistd.h>
#include "utilities/define.h"
#include "utilities/file.h"
#include "data_structures/city.h"
#include "networking/request.h"



// Command line arguments
typedef struct cla_s {
    char directoryPath[MAX_FILE_LEN];
    char ipAddress[MAX_IP_LENGTH];
    int start, end;
    int port;
} cla_t; 


/* Function Declarations */

// Controls command line arguments and assigns them to their global variables
void ctrlcla(int argc, char* argv[]);      
// Returns process id by using /proc/self directory
pid_t myGetPID();    
// Returns how many city directory are there inside dataset
int getCitySize();
// Returns the maximumu amount of transaction a servant can store
int getMaxTransactionCount();
// Takes a string in "1-10" format and sets cla_t object's start value as 1 and end value as 10
void setCityInterval(char* str);
// Reads all subdirectoires of current directory and assigns them into cityNames variable
void setCityNames();
// Allocates "cityCount" * city_t bytes of space for cities object
void allocateCities();
// Deallocates the space that is allocated for cities object
void deallocateCities();
// Stores all transaction informations into the cities object
void loadDataFromDisk();
// Gets the date in form of "01-01-2000" and inserts day month and year values into cityTransaction object
void tokenizeDate(int city, int dateIndex, char* date);
// Finds transaction information inside files with given date and inserts them into cities object
void insertTransactionInformations(int city, int cityIndex, int dateIndex, char* date);
// Converts "7282 BAG AYHTASSZ 180 7958000" string into id:7282, real-estate:BAG, ...
// After tokenizing, this function inserts them into transaction object
void tokenizeTransactionAndInsert(transaction_t* transaction, char* transaction_str);

// Creates a socket for client part and waits for server to connect
void createClientSocket();
// It sends port and dataset informations to the server.
// Then it closes the client socket
void sendServantInformation();

// Creates a socket for server
void createServerSocket();
// Waits until server sends a request
// The port it uses uniquely created for every servant
void waitForServerRequest();
// This is a thread function which receives a connectFD file descriptor to read incoming request
// After reading the request, sends back the response to server using p_connectFD and write
void* handleRequest(void* p_connectFD);
// Returns amount of transactions between date1 and date2 which given as parameter
// request_t object contains that information
int getTransactionCountBetweenDates(request_t* request);
// Gets a city name as parameter and returns it's index in the cityNames array
// Returns -1 when cityName is not in the cityNames array
int getCityIndex(char* cityName);
// Controls is date inside transaction is between date1 and date2(date informations are inside request object)
int isDateBetweenGivenDates(transaction_t* transaction, request_t* request);


// Prints all transactions belongs to this servant
void printTransactions(int start, int finish);


// Example output : "Servant 6672: loaded dataset, cities ADANA-ANKARA"
// Example output : "Servant 6672: listening at port 16001"
void printDialogue1(int servantPID, int port);
// Example output : "Servant 6672: termination message received, handled 10 requests in total."
void printDialogue2(int servantPID, int count);


// Initializes the signal handler using sigaction
// Also blocks every other signal except SIGINT using sigprocmask
void initializeSignalHandler();
// It only changes the value of given signal from 0 to 1
void handleSIG(int signal);

#endif