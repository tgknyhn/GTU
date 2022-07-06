#ifndef LINKED_LIST
#define LINKED_LIST
#include "city.h"

struct ll_node {
    city_t city;
    struct ll_node * next;
};

void insert(city_t newCity);
void getCity(int index);

#endif