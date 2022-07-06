#define _GNU_SOURCE
#include "../header/data_structures/city.h"
#include "../header/utilities/mytime.h"
#include "../header/utilities/file.h"
#include "../header/servant.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <dirent.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

// Global Variables
cla_t    args;                                          // Command line arguments
city_t * cities;                                        // Stores all transaction informations 
int      cityCount;                                     // How many cities are responsibility of this servant
char**   cityNames;                                    // All 81 cities and their names 
int      totalCityCount;
int      totalTransactionCount;

// Network
int socketFD;
int connectFD;
struct sockaddr_in serverAddr;
int addressLength = sizeof(serverAddr);
int ownPort;

// Synchronisation
pthread_mutex_t waitLock = PTHREAD_MUTEX_INITIALIZER;
int sigintFlag = NOSIGNAL;


/*  Steps
    -----
    1) Control command line arguments
    2) Initialize the signal handler
    3) Load data from disk
    4) Create socket as a client
    5) Send necessary informations(port, dataset) to the server
    6) Close that connection and create a server socket
    7) Listen from own port and wait for requests coming from the server
    8) When a request come, create a thread and send that request to it to handle
    9) Terminate program when SIGINT detected (don't forget cleaning up)
*/

int main(int argc, char *argv[])
{
    // Step 1
    ctrlcla(argc, argv);
    // Step 2
    initializeSignalHandler();
    // Step 3 
    loadDataFromDisk();
    // Step 4
    createClientSocket();
    // Step 5
    sendServantInformation();
    // Step 6
    createServerSocket();
    // Step 7
    waitForServerRequest(); // Step 8 is executed inside this function with "handleRequest"
    // Step 9
    deallocateCities();

    pthread_exit(0);
    return 0;
}


// Controls command line arguments and assigns them to their global variables
void ctrlcla(int argc, char* argv[]) {
    // Checking argument count
    if(argc != 9)
        exitWithError("Servant: Unsufficient argument count. Correct one should be : 9\n");

    int control_d = strcmp(argv[1], "-d");
    int control_c = strcmp(argv[3], "-c");
    int control_r = strcmp(argv[5], "-r");
    int control_p = strcmp(argv[7], "-p");

    // Controlling -d -c -r and -p characters
    if(control_d != 0 || control_c != 0 || control_r != 0 || control_p != 0)
        exitWithError("Servant: Invalid cla format. Valid example : \"./servant -d ../dataset -c 1-9 -r 127.0.0.1 -p 10000\".\n");

    // If characters are correct, then assign arguments to their variables
    strcpy(args.directoryPath, argv[2]);
    setCityInterval(argv[4]);
    strcpy(args.ipAddress, argv[6]);
    args.port = atoi(argv[8]);
    cityCount = args.end - args.start + 1;

    // Validating argument values
    if(args.port < 2000)
        exitWithError("Servant: Port value should be bigger than 2000 for safety. Terminating the server.\n");

    // Assigning unique own port value
    ownPort = args.start + args.end + 12000;
    // Get total city count
    totalCityCount = getCitySize();
    // Allocating space for cities object
    allocateCities();
    // Insert all city names into "cityNames" variable
    setCityNames();
    // Get total transaction count
    totalTransactionCount = getMaxTransactionCount();
    for(int i=0; i<cityCount; i++)
        cities[i].transactions = (transaction_t*)calloc(totalTransactionCount, sizeof(transaction_t));
}

// Returns process id by using /proc/self directory
pid_t myGetPID() {
    pid_t id;
    char buffer[32] = {0};

    readlink("/proc/self", buffer, sizeof(buffer));
    sscanf(buffer, "%d", &id);
    
    return id;
}

// Returns how many city directory are there inside dataset
int getCitySize() {
    // Declaring variables
    struct dirent **d_cityNames;
    int numberOfCities;
    // I removed 2 because in these city names both '.' and '..' are included
    numberOfCities = scandir(args.directoryPath, &d_cityNames, NULL, alphasort) - 2;
    // Deallocate the d_cityNames
    for(int i=0; i<numberOfCities+2; i++)
        free(d_cityNames[i]);
    free(d_cityNames);

    return numberOfCities;
}

// Returns number of date files per city
int getDatePerCity() {
    char directoryBuffer[100] = {0};
    strcpy(directoryBuffer, args.directoryPath);
    strcat(directoryBuffer, "/");
    strcat(directoryBuffer, cityNames[0]);
    // Declaring variables
    struct dirent **d_directories;
    int numberOfDates;
    // I removed 2 because in these directory names both '.' and '..' are included
    numberOfDates = scandir(directoryBuffer, &d_directories, NULL, alphasort) - 2;
    // Deallocate the d_cityNames
    for(int i=0; i<numberOfDates+2; i++)
        free(d_directories[i]);
    free(d_directories);
    return numberOfDates;
}

// Returns the first subdirectory of cityNames[0]
void getFirstDirName(char dirName[MAX_FILE_LEN]) {
    char directoryBuffer[100] = {0};
    strcpy(directoryBuffer, args.directoryPath);
    strcat(directoryBuffer, "/");
    strcat(directoryBuffer, cityNames[0]);
    // Declaring variables
    struct dirent **d_directories;
    int numberOfDates;
    // I removed 2 because in these directory names both '.' and '..' are included
    numberOfDates = scandir(directoryBuffer, &d_directories, NULL, alphasort) - 2;
    // Copy the directory name to dirName
    strcpy(dirName, directoryBuffer);
    strcat(dirName, "/");
    strcat(dirName, d_directories[2]->d_name);
    // Deallocate the d_cityNames
    for(int i=0; i<numberOfDates+2; i++)
        free(d_directories[i]);
    free(d_directories);
}

// Return the transaction count per date
int getTransactionPerDate() {
    char dirName[MAX_FILE_LEN] = {0};
    // Getting the name of first directory
    getFirstDirName(dirName);
    // Opening the directory
    FILE* fp = fopen(dirName, "r");
    // 
    char * transaction_str = NULL;
    size_t length = 0;
    int count = 0;
    while(getline(&transaction_str, &length, fp) != -1) {
        if(strlen(transaction_str) < 5)
            continue;
        count++;
    } 
    free(transaction_str);
    fclose(fp);
    return count;
}

// Returns the maximumu amount of transaction a servant can store in a city object
int getMaxTransactionCount() {
    // First we get date per city
    int datePerCity = getDatePerCity();
    // Then we get transaction per date
    int transactionPerDate = getTransactionPerDate();
    // Return their product
    return datePerCity * transactionPerDate;
}

// Takes a string in "1-10" format and sets cla_t object's start value as 1 and end value as 10
void setCityInterval(char* str) {
    // Get start-end values by tokenizing
    int start = atoi(strtok(str, "-"));
    int end   = atoi(strtok(NULL, "-"));

    // Set values to the argument struct
    args.start = start;
    args.end   = end;
}

// Reads all subdirectoires of current directory and assigns them into cityNames variable
void setCityNames() {
    // Declaring variables
    struct dirent **d_cityNames;
    int numberOfCities;
    // Reading the dataset directory and getting the list of city names into d_cityNames
    // I removed 2 because in these city names both '.' and '..' are included
    numberOfCities = scandir(args.directoryPath, &d_cityNames, NULL, alphasort) - 2;
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

// Allocates "cityCount" * city_t bytes of space for cities object
void allocateCities() {
    // Cities
    cities               = (city_t*)malloc(sizeof(city_t) * cityCount);
    // City Names
    cityNames = (char**)malloc(sizeof(char*) * totalCityCount);
    for(int i=0; i<totalCityCount; i++)
        cityNames[i] = (char*)malloc(sizeof(char) * MAX_CITY_LEN);
}

// Deallocate the space that is allocated for cities object
void deallocateCities() {
    for(int i=0; i<cityCount; i++)
        free(cities[i].transactions);
    free(cities);
    for(int i=0; i<totalCityCount; i++)
        free(cityNames[i]);
    free(cityNames);    
    pthread_exit(0);
}

// Stores all transaction informations into the cities object
void loadDataFromDisk() {
    // First doing some check
    if(cityCount > totalCityCount)
        exitWithError("Invalid city interval.\n");

    char directoryBuffer[100];
    // 
    struct dirent *dir;                          // points into directory entry
    DIR *dir_p = opendir(args.directoryPath);    // Opens given directory and return a DIR pointer
    for(int cityIndex=0, city=args.start-1; cityIndex<cityCount; cityIndex++, city++) {
        // Getting directory name into directory buffer
        strcpy(directoryBuffer, args.directoryPath);
        strcat(directoryBuffer, "/");
        strcat(directoryBuffer, cityNames[city]);
        // Open directory and get pointer
        DIR *dir_p = opendir(directoryBuffer);    // Opens given directory and return a DIR pointer
        
        for(int date=0; date<DATE_PER_CITY; date++) {
            // Get dirent object using dir_p
            dir = readdir(dir_p);
            // Check if subdirectory is equals to "." or ".." (we don't want these)
            if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) { date--; continue; }
            // Get transaction informations from every date and insert them into cities object
            insertTransactionInformations(city, cityIndex, date, dir->d_name);
            // Turn date from 01-01-2000 to day:1 month:1 year:2000 (also does error check)
            tokenizeDate(cityIndex, date, dir->d_name);
        }
        free(dir_p);
    }
    // Closing the dataset directory
    closedir(dir_p);  
}

// Finds transaction information inside files with given date and inserts them into cities object
void insertTransactionInformations(int city, int cityIndex, int dateIndex, char* date) {
    // Getting directory
    char directoryBuffer[150];
    // Appending names
    strcpy(directoryBuffer, args.directoryPath);
    strcat(directoryBuffer, "/");
    strcat(directoryBuffer, cityNames[city]);
    strcat(directoryBuffer, "/");
    strcat(directoryBuffer, date);  // This is final form (For example : "dataset/ADANA/01-05-2073")

    // Opening file at directoryBuffer
    FILE* fp = fopen(directoryBuffer, "r");

    if(fp == NULL)
        exitWithError("Servant: Error occured while calling insertTransactionInformations.\n");

    char * transaction_str = NULL;
    size_t length = 0;

    int start = (dateIndex * DATE_PER_CITY);
    int end = (dateIndex * DATE_PER_CITY) + 100;


    for(int i=start; i<end; i++) {
        // Getting transaction
        getline(&transaction_str, &length, fp); 
        if(strlen(transaction_str) < 6)
            continue;
        else
            transaction_str[strlen(transaction_str)-1] = '\0';
        // Insert transaction into cities object
        tokenizeTransactionAndInsert(&cities[cityIndex].transactions[i], transaction_str);
    }

    // Free the line
    free(transaction_str);
    // Closing the file
    fclose(fp);
}

// Converts "7282 BAG AYHTASSZ 180 7958000" string into id:7282, real-estate:BAG, ...
// After tokenizing, this function inserts them into transaction object
void tokenizeTransactionAndInsert(transaction_t* transaction, char* transaction_str) {
    transaction->id = atoi(strtok(transaction_str, " "));   // Transaction ID
    strcpy(transaction->realEstate, strtok(NULL, " "));     // Real Estate
    strcpy(transaction->streetName, strtok(NULL, " "));     // Street Name
    transaction->surfaceSqMt = atoi(strtok(NULL, " "));     // Surface in Square Meter
    transaction->price = atoi(strtok(NULL, " "));           // Price of the transaction

    if(transaction->id <= 0)
        exitWithError("\nTransaction ID can't be lower than 1\n");
}

// Gets the date in form of "01-01-2000" and inserts day month and year values into cityTransaction object
void tokenizeDate(int city, int dateIndex, char* date) {
    int day   = atoi(strtok(date, "-"));
    int month = atoi(strtok(NULL, "-"));
    int year  = atoi(strtok(NULL, "-"));

    if(day < 1 || day > 30)
        exitWithError("Servant: Day value is wrong. Days are between 1-30. Terminating servant process.\n");
    if(month < 1 || month > 12)
        exitWithError("Servant: Month value is wrong. Months are between 1-12. Terminating servant process.\n");
    if(year < 2000)
        exitWithError("Servant: Year value is wrong. Years start with 2000. Terminating servant process.\n");
    
    int start = (dateIndex * DATE_PER_CITY);
    int end = (dateIndex * DATE_PER_CITY) + 10;

    for(int i=start; i<end; i++) {
        cities[city].transactions[i].day   = day;
        cities[city].transactions[i].month = month;
        cities[city].transactions[i].year  = year;
    }
}

// Prints all transactions belongs to this servant
void printTransactions(int start, int finish) {
    for(int i=start; i<finish; i++) {
        for(int j=0; j<10; j++) {
            for(int k=0; k<10; k++) {
                int index = j*10 + k;
                printf("%d-", cities[i].transactions[index].day);
                printf("%d-", cities[i].transactions[index].month);
                printf("%d\t", cities[i].transactions[index].year);
                printf("%d ", cities[i].transactions[index].id);
                printf("%s ", cities[i].transactions[index].realEstate);
                printf("%s ", cities[i].transactions[index].streetName);
                printf("%d ", cities[i].transactions[index].surfaceSqMt);
                printf("%d\n", cities[i].transactions[index].price);
            }
            printf("\n");
        }
    }
}

// Creates a socket for client part and waits for server to connect
void createClientSocket() {
    // Setting serverAddr attributes
    serverAddr.sin_family      = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(args.ipAddress); 
    serverAddr.sin_port        = htons(args.port);

    // 1) Creating socket and getting the file descriptor
    if((socketFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exitWithError("Servant: Error occured while getting socketFD.\n");
    
    // 2) Connecting client to the ipc and waiting for server
    if(connect(socketFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        exitWithError("Servant: Error occured while connecting client socket.\n");
}

// Creates a socket for server
void createServerSocket() {
    // Setting serverAddr attributes
	serverAddr.sin_family      = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port        = htons(ownPort);

    // 1) Creating socket and getting the file descriptor
	if((socketFD = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        exitWithError("Servant: Error occurred while calling socket function.\n");
        
    // 2) Make server address rebindable
    int opt = 1;
    if (setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        exitWithError("Servant: Error occured while calling setsockopt.\n");

    // 3) Associating the server address with the socket using file descriptor
	if(bind(socketFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        exitWithError("Servant: Error occured while binding the socket.\n");

    // 4) Making socket go silent and wait for a connection 
	if(listen(socketFD, 20) < 0) 
        exitWithError("Servant: Error occured while listening the socket.\n");
}

// It sends port and dataset informations to the server.
// Then it closes the client socket
void sendServantInformation() {
    // Allocating space for servantInfo object
    servantInfo_t * info = (servantInfo_t*)calloc(1, sizeof(servantInfo_t));
    // Filling the serventInfo struct
    info->start      = args.start;
    info->end        = args.end;
    info->port       = ownPort;
    info->servantPID = myGetPID();
    strcpy(info->ipAddress, args.ipAddress);
    strcpy(info->directory, args.directoryPath);
    // Send object to the server
    write(socketFD, info, sizeof(*info));
    // Close the socket
    close(socketFD);
    // Free the info address
    free(info);
}

// Waits until server sends a request
// The port it uses uniquely created for every servant
void waitForServerRequest() {
    // Initializing attributes for detaching
    pthread_attr_t detached;
    pthread_attr_init(&detached);
    pthread_attr_setdetachstate(&detached, PTHREAD_CREATE_DETACHED);
    // Total request count which will be printed to the console
    int requestCount = 0;
    // Print out cities you selected and port the servant is listening at
    printDialogue1(myGetPID(), ownPort);
    for(;;) {
        // Getting new socket and accepting requests from clients
        if ((connectFD = accept(socketFD, (struct sockaddr*)&serverAddr, (socklen_t*)&addressLength)) < 0) {
            if(sigintFlag == SIGNAL)
                break;
            exitWithError("Servant: Error occurred while accepting the request.\n");
        }
        int* p_connectFD = (int*)malloc(sizeof(int));
        // Assign new socket fd into an int pointer
        *p_connectFD = connectFD;
        // incrementing the request count by 1
        requestCount += 1;
        // Create a new thread to handle request coming from the server
        pthread_t thread;
        pthread_create(&thread, &detached, handleRequest, p_connectFD);
    }
    pthread_attr_destroy(&detached);
    printDialogue2(myGetPID(), requestCount);
}

// This is a thread function which receives a connectFD file descriptor to read incoming request
// After reading the request, sends back the response to server using p_connectFD and write
void* handleRequest(void* p_connectFD) {
    int requestFD = *((int*)p_connectFD); free(p_connectFD);
    void* buffer  = (void*)calloc(sizeof(void), 1024);
    // Reading the request into void buffer
    read(requestFD, buffer, 1024);
    // Casting request into request_t 
    request_t * request = (request_t*)buffer;
    // Getting transactiount between given dates
    int transactionCount = getTransactionCountBetweenDates(request);
    // Sending transaction count information back to server
    sprintf((char*)buffer, "%d", transactionCount);
    write(requestFD, buffer, strlen(buffer));
    
    // Free the allocated space and close the file descriptor
    free(buffer);
    close(requestFD);
    // Returning NULL since we don't need to return anything
    return NULL;
}

// Returns amount of transactions between date1 and date2 which given as parameter
// request_t object contains that information
int getTransactionCountBetweenDates(request_t* request) {
    int count = 0;
    if(strlen(request->city) != 0) {
        // Getting index of the city 
        int cityIndex = getCityIndex(request->city);
        // Directly accesing to that city
        for(int i=0; i<TRANSACTION_PER_DATE*DATE_PER_CITY; i++) {
            // Comparing real estates
            int realEstateMatch = strcmp(cities[cityIndex].transactions[i].realEstate, request->realEstate) == 0 ? 1 : 0;
            // Comparing dates
            int dateIsBetweenGivenDates = isDateBetweenGivenDates(&cities[cityIndex].transactions[i], request);
            // Looking at the result and incrementing count if both are correct
            if(realEstateMatch && dateIsBetweenGivenDates)
                count++;
        }
    } else {
        for(int cityIndex=0; cityIndex<cityCount; cityIndex++) {
            // Directly accesing to that city
            for(int i=0; i<TRANSACTION_PER_DATE*DATE_PER_CITY; i++) {
                // Comparing real estates
                int realEstateMatch = strcmp(cities[cityIndex].transactions[i].realEstate, request->realEstate) == 0 ? 1 : 0;
                // Comparing dates
                int dateIsBetweenGivenDates = isDateBetweenGivenDates(&cities[cityIndex].transactions[i], request);
                // Looking at the result and incrementing count if both are correct
                if(realEstateMatch && dateIsBetweenGivenDates)
                    count++;
            }
        }
    }
    return count;
}

// Controls is date inside transaction is between date1 and date2(date informations are inside request object)
int isDateBetweenGivenDates(transaction_t* transaction, request_t* request) {
    // First we check if the transaction date is after start date
    if(transaction->year < request->yearStart)
        return 0;
    else if(transaction->year == request->yearStart) {
        if(transaction->month < request->monthStart)
            return 0;
        else if(transaction->month == request->monthStart) 
            if(transaction->day < request->dayStart)
                return 0;
    }

    // Second and finally, we check if the transaction date is before end date
    if(transaction->year > request->yearEnd)
        return 0;
    else if(transaction->year == request->yearEnd) {
        if(transaction->month > request->monthEnd)
            return 0;
        else if(transaction->month == request->monthEnd) 
            if(transaction->day > request->dayEnd)
                return 0;
    }

    // If still code reaches here, then it means the transaction date is between date1 and date2
    // So we can return 1 which is true
    return 1; 
}

// Gets a city name as parameter and returns it's index in the cityNames array
// Returns -1 when cityName is not in the cityNames array
int getCityIndex(char* cityName) {
    if(cityName == NULL)
        return -1;

    for(int i=0; i<TOTAL_CITY_COUNT; i++) {
        if(strcmp(cityName, cityNames[i]) == 0)
            return i - args.start + 1;
    }
    
    return -1;
}


// Example output : "Servant 6672: loaded dataset, cities ADANA-ANKARA"
// Example output : "Servant 6672: listening at port 16001"
void printDialogue1(int servantPID, int port) {
    char timeSTR[100]; getCurrentTime(timeSTR);
    // Locking
    pthread_mutex_lock(&waitLock);
    // Printing first city only
    printf("%s Servant %d: loaded dataset, cities %s", timeSTR, servantPID, cityNames[args.start-1]);
    // Printing rest of it
    for(int i=args.start; i<args.end; i++)
        printf("-%s", cityNames[i]);
    printf("\n");
    printf("%s Servant %d: listening at port %d\n", timeSTR, servantPID, port);
    // Unlocking
    pthread_mutex_unlock(&waitLock);
}

// Example output : "Servant 6672: termination message received, handled 10 requests in total."
void printDialogue2(int servantPID, int count) {
    char timeSTR[100]; getCurrentTime(timeSTR);
    printf("%s Servant %d: termination message received, handled %d requests in total.\n", timeSTR, servantPID, count);
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