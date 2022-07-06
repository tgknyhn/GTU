#ifndef FILE_H
#define FILE_H

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Opens the file with given path and flag(read or write [r|w])
int openFile(char * filePath, char * fileType);
//
int readFromFile(char * filePath, char * fileContext, int fileLength);
int readFromFileFD(int fileDescriptor, char * fileContext, int readSize);
//
int writeToFile(char * filePath, char* fileContext);
int writeToFileFD(int fileDescriptor, char* fileContext);
//
void closeFile(int fileDescriptor);
//
int getFileLength(char * filePath);
//
void lockFileForWrite(int fileDescriptor);
//
void unlockFileForWrite(int fileDescriptor);

#endif