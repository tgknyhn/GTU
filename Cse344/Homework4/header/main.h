#ifndef MAIN_H
#define MAIN_H

#define PATH_LEN 100
#define MAX_BUFFER_SIZE 200
#define SEM_PATH "/tmp"
#define NOSIGNAL 0
#define SIGNAL 1

typedef union semun {                   /* Used in calls to semctl() */
    int                 val;
    struct semid_ds *   buf;
    unsigned short *    array;
    #if defined(__linux__)
        struct seminfo *    __buf;
    #endif
} semun_t;

//
void ctrlcla(int argc, char* argv[]);
//
void initializeSysmtemVsemaphores();
//
void startSupplier();
void startConsumers();
//
void* supplier(void* arg);
void* consumer(void* arg);
//
void joinThreads();
//
char* getTimeBuffer();
//
void printBeforeDelivery(char buffer);
void printAfterDelivery(char buffer);
//
void printBeforeConsuming(int id, int iteration);
void printAfterConsuming(int id, int iteration);
//
void printSupplierExit();
void printConsumerExit(int id);
//
void initializeSignalHandler();
void handleSIG(int signal);

#endif