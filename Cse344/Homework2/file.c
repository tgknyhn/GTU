#include "file.h"

#define READ_FLAGS  (O_RDONLY)                      // Open with read permission or create the file
#define WRITE_FLAGS (O_CREAT | O_WRONLY | O_TRUNC)  // Open with write permission or create the file
#define FILE_PERMS 0777                             // Reading/Writing/Execution permissions for all users 

void checkErrors(int result);

int openFile(char * filePath, char * fileType) {
    int fileDescriptor;
    if(strcmp(fileType, "r") == 0) {
        fileDescriptor = open(filePath, READ_FLAGS, FILE_PERMS); 
    }
    else if(strcmp(fileType, "w") == 0) {
        fileDescriptor = open(filePath, WRITE_FLAGS, FILE_PERMS); 
    }
    // Checking for errors
    checkErrors(fileDescriptor);

    return fileDescriptor;
}

int readFromFile(char * filePath, char * fileContext, int fileLength) {
    // Opening the file with read flag
    int fd = openFile(filePath, "r");
    // Reading the file context
    int length = read(fd, fileContext, fileLength);
    // Checking for errors
    checkErrors(length);
    // Closing the file
    closeFile(fd);
    // Returning the length
    return length;
}

int readFromFileFD(int fileDescriptor, char * fileContext, int readSize) {
    // Reading the file context
    int length = read(fileDescriptor, fileContext, readSize);
    // Checking for errors
    checkErrors(length);
    // Returning the length
    return length;
}

int writeToFile(char * filePath, char* fileContext) {
    // Write Lock struct
    struct flock lockStruct;
    // Initializing the flock struct
    memset(&lockStruct, 0, sizeof(lockStruct));
    // Opening the file with read flag
    int fd = openFile(filePath, "w");
    // locking the file for writing
    lockFileForWrite(fd);
    // Writing to the file 
    int length = write(fd, fileContext, strlen(fileContext));
    // Control for errors
    checkErrors(length);
    // Closing the file
    closeFile(fd);
    // Unlocking the file for writing
    unlockFileForWrite(fd);
    // Returning the length
    return length;
}

int writeToFileFD(int fileDescriptor, char* fileContext) {
    // Writing to the file 
    int length = write(fileDescriptor, fileContext, strlen(fileContext));
    // Control for errors
    checkErrors(length);
    // Returning the length
    return length;
}

int getFileLength(char * filePath) {
    int fd = openFile(filePath, "r");
    // Getting length of the file
    long int length = lseek(fd, 0, SEEK_END); 
    lseek(fd, 0, SEEK_SET);
    // Closing the file
    closeFile(fd);
    // Returning the length
    return length;
}

void closeFile(int fileDescriptor) {
    int result = close(fileDescriptor);

    checkErrors(result);
}


void lockFileForWrite(int fileDescriptor) {
    // Write Lock struct
    struct flock lockStruct;
    // Initializing the flock struct
    memset(&lockStruct, 0, sizeof(lockStruct));
    // Setting type of the lock
    lockStruct.l_type = F_WRLCK;
    // Placing the lock
    if(fcntl(fileDescriptor, F_SETLKW, &lockStruct) == -1) {
        perror("Locking operation failed.");
        exit(EXIT_FAILURE);
    }
}

void unlockFileForWrite(int fileDescriptor) {
    // Write Lock struct
    struct flock lockStruct;
    // Initializing the flock struct
    memset(&lockStruct, 0, sizeof(lockStruct));
    // Setting type of the lock
    lockStruct.l_type = F_UNLCK;
    // Placing the lock
    if(fcntl(fileDescriptor, F_SETLKW, &lockStruct) == -1) {
        perror("Unlocking operation failed.");
        exit(EXIT_FAILURE);
    }

}

void checkErrors(int result) {
    if(result == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
}