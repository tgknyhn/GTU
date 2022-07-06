/* 

#define N 100000
int count = 0;
int buffer[N];
int producerFlag = 0;
int consumerFlag = 1;

void sleep(int & flag) {
    while(flag);
    // Setting flag back to normal
    flag = true;
}


// Producer Task
void taskP()
{
    // Initializing the item
    int item = 0;

    while(true) {
        // Checking if buffer is full, if it is wait until it is not
        if(count == N) sleep(producerFlag);
        
        if(!producerFlag) {
            enter_region(0);
            item++; if(item == 200) item = 0;
            // Inserting the item
            buffer[count] = item;
            // Incrementing the count
            count++;
            // If this is the first item added, wake up the consumer
            if(count == 1) consumerFlag = 0;
            leave_region(0);
        }
        myNop(10000);

    }
}

// Consumer Task
void taskC()
{
    // Declaring the item
    int item;
    char str[10];
    
    while(true) {
        // Check buffer, if it is empty get into while loop until it is not
        if(count == 0) sleep(consumerFlag);

        if(!consumerFlag) {
            enter_region(1);
            // Taking item out of the buffer
            item = buffer[count];
            // Decrementing the count
            count--;
            // if buffer is not full anymore, then wakeup the producer
            if(count == N-1) producerFlag = 0;
            // Print the result
            printf(taskManager.myItoa(item, str));
            printf("\t");
            printf(taskManager.myItoa(count, str));
            printf("\n");
            leave_region(1);
        }
        myNop(10000);

    }
}

void taskP2()
{
    // Initializing the item
    int item = 0;

    while(true) {
        // Checking if buffer is full, if it is wait until it is not
        if(count == N) sleep(producerFlag);
        
        item++; if(item == 200) item = 0;
        // Inserting the item
        buffer[count] = item;
        // Incrementing the count
        count++;
        // If this is the first item added, wake up the consumer
        if(count == 1) consumerFlag = 0;
        myNop(10000);

    }
}

// Consumer Task
void taskC2()
{
    // Declaring the item
    int item;
    char str[10];
    
    while(true) {
        // Check buffer, if it is empty get into while loop until it is not
        if(count == 0) sleep(consumerFlag);

        // Taking item out of the buffer
        item = buffer[count];
        // Decrementing the count
        count--;
        // if buffer is not full anymore, then wakeup the producer
        if(count == N-1) producerFlag = 0;
        // Print the result
        printf(taskManager.myItoa(item, str));
        printf("\n");
        // my sleep function
        myNop(10000);

    }
} 

*/