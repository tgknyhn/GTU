#ifndef FILE_H
#define FILE_H

#define READ_FLAGS  (O_RDONLY)                      // Open with read permission or create the file
#define WRITE_FLAGS (O_CREAT | O_WRONLY | O_TRUNC)  // Open with write permission or create the file
#define FILE_PERMS 0777                             // Reading/Writing/Execution permissions for all users 
#define MAX_FILE_LEN 100

// Opens the file with given path and flag(read or write [r|w])
int openFile(char * filePath, char * fileType);
//
int readFromFile(char * filePath, void * fileContext, int fileLength);
int readFromFileFD(int fileDescriptor, void * fileContext, int readSize);
//
int writeToFile(char * filePath, void* fileContext);
int writeToFileFD(int fileDescriptor, void* fileContext, int size);
int writeToFile_wFlag(char * filePath, void* fileContext, void* flag);
int writeToFifo(char * filePath, void* fileContext, int size);
//
void closeFile(int fileDescriptor);
//
int getFileLength(char * filePath);
//
void lockFileForWrite(int fileDescriptor);
//
void unlockFileForWrite(int fileDescriptor);
//
void exitWithError(char* errMsg);
// Fifo
void createFifo(char* fifoPath);
void removeFifo(char* fifoPath);

#endif