#ifndef FILE_H
#define FILE_H

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int  getFileLength      (char * filePath);
int  openFile           (char * filePath, char * fileType);
int  readFromFile       (char * filePath, char * fileContext, int fileLength);
int  writeToFile        (char * filePath, char* fileContext);
void closeFile          (int fileDescriptor);
void lockFileForWrite   (int fileDescriptor, struct flock * lockStruct);
void unlockFileForWrite (int fileDescriptor, struct flock * lockStruct);

#endif