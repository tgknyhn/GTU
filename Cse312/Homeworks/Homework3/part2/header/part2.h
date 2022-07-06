#ifndef PART2_H
#define PART2_H

#define DATE_LENGTH 100
#define DATA_LENGTH 100
#define NAME_LENGTH 41
#define KB_AS_BYTES 1024
#define MB_AS_BYTES (KB_AS_BYTES * 1024)
#define MAX_FILESYSTEM_SIZE (16 * MB_AS_BYTES)

typedef struct superblock_s {
    char magicNumber;
    int blockSize;
    int blockCount;
    int directoryCount;
    int directoryBlockCount;
    int iNodeBlockCount;
    int startOfiNodeBlock;
    int startOfRootDirectory;
    int startOfDirectoryBlock;
    int startOfDataBlock;
    int startOfFreeSpace;
} superblock_t;

typedef struct iNode_s {
    int index;
    int fileSize;
    char fileName[NAME_LENGTH];
    char lastModification[DATE_LENGTH];
    int dataBlock1;
    int dataBlock2;
    int dataBlock3;
    int indirectDataSingle;
    int indirectDataDouble;
    int indirectDataTriple;
} iNode_t;

typedef struct directoryBlock_s {
    int index;
    char directoryName[NAME_LENGTH];
    char data[DATA_LENGTH];
    int childDirectoryBlock1;
    int childDirectoryBlock2;
} directoryBlock_t;

typedef struct rootDirectory_s {
    int dataBlocks[2];
    int directoryBlocks[2];
} rootDirectory_t;

typedef struct freeSpaceBlock_s {
    int numberOfFreeBlocks;
    int numberOfFreeINodeBlocks;
    int numberOfFreeDirectoryBlocks;
    int numberOfFreeDataBlocks;
    int firstFreeINodeBlock;
    int firstFreeDirectoryBlock;
    int firstFreeDataBlock;
    int fileCountAtRootDirectory;
    int directoryCountAtRootDirectory;
} freeSpaceBlock_t;

typedef struct fileSystem_s {
    superblock_t     superblock;
    rootDirectory_t  rootDirectoryBlock;
    freeSpaceBlock_t freeSpaceBlock;
    iNode_t          * iNodeBlocks;
    directoryBlock_t * directoryBlocks;
    char             ** dataBlocks;
} fileSystem_t;


void ctrcla(int argc, char *argv[]);
void exitWithError(char * errorMessage);

void fillFileSystemStruct();
void exportFileSystem();

#endif