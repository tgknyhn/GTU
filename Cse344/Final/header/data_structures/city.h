#ifndef CITY_H
#define CITY_H
#include "../utilities/define.h"


typedef struct transaction_s {
    int id;
    char realEstate[MAX_REAL_ESTATE_LEN];
    char streetName[MAX_STREET_LEN];
    int surfaceSqMt;
    int price;
    // Date
    int day;
    int month;
    int year;
} transaction_t;

typedef struct city_s {
    transaction_t* transactions;
} city_t;

#endif