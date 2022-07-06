#include "../../header/data_structures/queue.h"

struct node* head = NULL;
struct node* tail = NULL;

// Adds given integer value to end of the queue
void enqueue(int data) {
    // Creating a new node to insert connectFD
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    // Inserting new values to that node
    newNode->data = data;
    newNode->next = NULL;
    // If this is the first node, than add it to head and tail of the queue
    if(head == NULL) {
        head = newNode;
        tail = newNode;
    }
    // If it is not the first node than make previous rear->next to point newNode
    // And change current rear to the newNode
    else {
        tail->next = newNode;
        tail = newNode;
    }
}

// Removes first data in the queue and returns it
// Returns -1 when queue is empty
int dequeue() {
    int returnValue = -1;
    // If head node is NULL it means queue is empty, so return -1 which indicates an error
    if(head == NULL)
        return -1;
    else {
        // First we get return value
        returnValue = head->data;
        // Then we assign head node as trash since we no longer need it
        struct node * trash = head;
        // Make head node to point next node after head 
        // Because we don't want the head node anymore
        if(head->next == NULL) { // If head->next is null then it means head is the last node available 
            head = NULL;
            tail = NULL;
        }
        else 
            head = head->next;
        //deallocate the trash node
       free(trash);
    }
    return returnValue;
}