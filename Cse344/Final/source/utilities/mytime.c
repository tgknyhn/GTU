#include "../../header/utilities/mytime.h"
#include <stdio.h>
#include <time.h>

// Gets an empty timeSTR and turns it into for ex: "01/01/2000 | 14:43:02 ->"
void getCurrentTime(char* timeSTR) {
    // Getting time structures
    time_t rawtime;
    struct tm * timeinfo;
    // Initializing the structures
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    // Assigning time values to int variables
	int day   = timeinfo->tm_mday;		
	int month = timeinfo->tm_mon + 1;
	int year  = timeinfo->tm_year + 1900;
	int hour  = timeinfo->tm_hour;
	int min   = timeinfo->tm_min; 
	int sec   = timeinfo->tm_sec;
    // Passing all time information to timeSTR
    sprintf(timeSTR, "%d/%d/%d | %d:%d:%d ->",
        day, month, year,
        hour, min, sec);
}