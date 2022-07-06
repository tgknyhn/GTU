#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "../header/file.h"

int openFile(char * filePath, char * fileType) {
    int fileDescriptor;
    if(strcmp(fileType, "r") == 0) {
        fileDescriptor = open(filePath, READ_FLAGS, FILE_PERMS); 
    }
    else if(strcmp(fileType, "rc") == 0) {
        fileDescriptor = open(filePath, (O_CREAT | O_RDONLY), FILE_PERMS); 
    }
    else if(strcmp(fileType, "wo") == 0) {
        fileDescriptor = open(filePath, O_WRONLY, FILE_PERMS); 
    }
    else if(strcmp(fileType, "wop") == 0) {
        fileDescriptor = open(filePath, O_WRONLY | O_APPEND, FILE_PERMS); 
    }
    else if(strcmp(fileType, "w") == 0) {
        fileDescriptor = open(filePath, WRITE_FLAGS, FILE_PERMS); 
    }

    return fileDescriptor;
}

int readFromFile(char * filePath, void * fileContext, int fileLength) {
    // Opening the file with read flag
    int fd = openFile(filePath, "r");
    // Reading the file context
    int length = read(fd, fileContext, fileLength);
    // Checking for errors
    if(length == -1)
        exitWithError("Read operation failed\n");
    // Closing the file
    closeFile(fd);
    // Returning the length
    return length;
}

int readFromFileFD(int fileDescriptor, void * fileContext, int readSize) {
    // Reading the file context
    int length = read(fileDescriptor, fileContext, readSize);
    // Checking for errors
    if(length == -1)
        exitWithError("Read operation failed\n");
    // Returning the length
    return length;
}

int writeToFile(char * filePath, void* fileContext) {
    // Opening the file with read flag
    int fd = openFile(filePath, "w");
    // locking the file for writing
    lockFileForWrite(fd);
    // Writing to the file 
    int length = write(fd, fileContext, strlen(fileContext));
    // Control for errors
    if(length == -1)
        exitWithError("Read operation failed\n");
    // Unlocking the file for writing
    unlockFileForWrite(fd);
    // Closing the file
    closeFile(fd);
    // Returning the length
    return length;
}

int writeToFile_wFlag(char * filePath, void* fileContext, void* flag) {
    // Opening the file with read flag
    int fd = openFile(filePath, flag);
    // locking the file for writing
    lockFileForWrite(fd);
    // Writing to the file 
    int length = write(fd, fileContext, strlen(fileContext));
    // Control for errors
    if(length == -1)
        exitWithError("Read operation failed\n");
    // Unlocking the file for writing
    unlockFileForWrite(fd);
    // Closing the file
    closeFile(fd);
    // Returning the length
    return length;
}

int writeToFileFD(int fileDescriptor, void* fileContext, int size) {
    // locking the file for writing
    lockFileForWrite(fileDescriptor);
    // Writing to the file 
    int length = write(fileDescriptor, fileContext, size);
    // Control for errors
    if(length == -1)
        exitWithError("Read operation failed\n");
    // Unlocking the file for writing
    unlockFileForWrite(fileDescriptor);
    // Returning the length
    return length;
}

int writeToFifo(char * filePath, void* fileContext, int size) {
    // Opening the file with read flag
    int fd = openFile(filePath, "w");
    // locking the file for writing
    lockFileForWrite(fd);
    // Writing to the file 
    int length = write(fd, fileContext, size);
    // Control for errors
    if(length == -1)
        exitWithError("Read operation failed\n");
    // Unlocking the file for writing
    unlockFileForWrite(fd);
    // Closing the file
    closeFile(fd);
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

    if(result == -1) 
        exitWithError("Error occured while closing the file.\n");
}


void lockFileForWrite(int fileDescriptor) {
    // Write Lock struct
    struct flock lockStruct;
    // Initializing the flock struct
    memset(&lockStruct, 0, sizeof(lockStruct));
    // Setting type of the lock
    lockStruct.l_type = F_WRLCK;
    // Placing the lock
    if(fcntl(fileDescriptor, F_SETLKW, &lockStruct) == -1)
        exitWithError("Locking operation failed.\n");
}

void unlockFileForWrite(int fileDescriptor) {
    // Write Lock struct
    struct flock lockStruct;
    // Initializing the flock struct
    memset(&lockStruct, 0, sizeof(lockStruct));
    // Setting type of the lock
    lockStruct.l_type = F_UNLCK;
    // Placing the lock
    if(fcntl(fileDescriptor, F_SETLKW, &lockStruct) == -1)
        exitWithError("Unlock operation failed.\n");
}

void createFifo(char* fifoPath) {
    umask(0);
	if (mkfifo(fifoPath, FILE_PERMS) == -1 && errno != EEXIST) 
        exitWithError("Error occured while creating the FIFO file.\n");
}

void removeFifo(char* fifoPath) {
    if(unlink(fifoPath) == -1)
        exitWithError("Error occured while removing the FIFO file.\n");
}

void exitWithError(char* errMsg) {
    // Getting size of the error message
    int errMsgSize = strlen(errMsg);
    // Printing it out with stderr
    write(STDERR_FILENO, errMsg, errMsgSize);
    perror("");
    // Terminating the program after error msg
    exit(EXIT_FAILURE);
}