#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "file.h"

// Define Variables
#define INVERTIBLE 1
#define NOT_INVERTIBLE 0
#define CLIENT_FIFO_TEMPLATE "/tmp/ClientFIFO_%ld"
#define CLIENT_FIFO_LENGTH   (sizeof(CLIENT_FIFO_TEMPLATE) + 20)

typedef struct request_s
{
    pid_t pid;
    char pathToDataFile[MAX_FILE_LEN];
} request_t;

typedef struct response_s
{
    int isInvertible;
} response_t;

typedef struct result_s
{
    int total;
    int invertible;
    int nonInvertible;
} result_t;
