#ifndef REQUEST_H
#define REQUEST_H
#include "../utilities/define.h"

typedef struct request_s {
    char requestType[50];
    char realEstate[MAX_REAL_ESTATE_LEN];
    int dayStart, dayEnd;
    int monthStart, monthEnd;
    int yearStart, yearEnd;
    char city[MAX_CITY_LEN];
    int id;
    char requestStr[100];
} request_t;

typedef struct servantInfo_s {
    int port;                        // Port address each servant will wait at. It will be unique for every servant
    int start;                       // Start index of cities (Ex: Aksaray - 5)
    int end;                         // End indexx of cities (Ex: Ankara - 7) so this servant responsible for Aksaray, Amasya and Ankara
    char ipAddress[MAX_IP_LENGTH];   // ip address that servant uses
    int servantPID;                  // For users (it will be printed out)
    char directory[MAX_FILE_LEN];    // Path to dataset
} servantInfo_t;

#endif