#ifndef QUEUE_H
#define QUEUE_H
#include <stdlib.h>

struct node {
    struct node* next;
    int data;
};

// Adds given integer value to end of the queue
void enqueue(int data);
// Removes first data in the queue and returns it
// Returns -1 when queue is empty
int dequeue();

#endif