#include "../header/part2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

int  block_size;
char system_name[NAME_LENGTH];
fileSystem_t file_system;

int main(int argc, char *argv[])
{
    // Control command line arguments
    ctrcla(argc, argv);
    // Fill file_system struct
    fillFileSystemStruct();
    // Export file_system struct as system_name
    exportFileSystem();

    return 0;
}

void ctrcla(int argc, char *argv[]) {
    if(argc != 3)
        exitWithError("Incorrect command line argument count. Valid usage: \"makeFileSystem 4 mySystem.dat\"");

    block_size = atoi(argv[1]);
    strcpy(system_name, argv[2]);

    if(block_size < 1)
        exitWithError("Unsufficient block size.\n");
}

void fillFileSystemStruct() {
    // Filling the superblock
    file_system.superblock.magicNumber = 0;
    file_system.superblock.blockSize  = block_size * KB_AS_BYTES;
    file_system.superblock.blockCount = MAX_FILESYSTEM_SIZE / file_system.superblock.blockSize;
    file_system.superblock.directoryBlockCount = file_system.superblock.blockCount / 10;
    file_system.superblock.directoryCount = (file_system.superblock.blockCount / 10) * 2;
    file_system.superblock.iNodeBlockCount = (file_system.superblock.blockCount / 10) * 2;
    file_system.superblock.startOfRootDirectory = 0;
    file_system.superblock.startOfFreeSpace = 128;
    file_system.superblock.startOfDataBlock = file_system.superblock.startOfFreeSpace + (32*9);
    file_system.superblock.startOfiNodeBlock = file_system.superblock.startOfDataBlock + file_system.superblock.blockSize * (file_system.superblock.blockCount - file_system.superblock.directoryBlockCount - 3);
    file_system.superblock.startOfDirectoryBlock = file_system.superblock.startOfiNodeBlock + (((8*32) + 141) * file_system.superblock.iNodeBlockCount);
    // Filling the root directory block
    for(int i=0; i<2; i++) {
        file_system.rootDirectoryBlock.dataBlocks[i] = 0;
        file_system.rootDirectoryBlock.directoryBlocks[i] = 0;
    }
    // Filling the free space block
    file_system.freeSpaceBlock.directoryCountAtRootDirectory = 0;
    file_system.freeSpaceBlock.fileCountAtRootDirectory = 0;
    file_system.freeSpaceBlock.firstFreeDataBlock = file_system.superblock.startOfFreeSpace;
    file_system.freeSpaceBlock.firstFreeDirectoryBlock = file_system.superblock.startOfDirectoryBlock;
    file_system.freeSpaceBlock.firstFreeINodeBlock = file_system.superblock.startOfiNodeBlock;
    file_system.freeSpaceBlock.numberOfFreeBlocks = file_system.superblock.blockCount;
    file_system.freeSpaceBlock.numberOfFreeDataBlocks = file_system.superblock.blockCount - file_system.superblock.directoryBlockCount - 3;
    file_system.freeSpaceBlock.numberOfFreeDirectoryBlocks = file_system.superblock.directoryBlockCount;
    file_system.freeSpaceBlock.numberOfFreeINodeBlocks = file_system.superblock.iNodeBlockCount;
}

void exportFileSystem() {
    // Open file
    FILE * fp = fopen(system_name, "w");
    // exporting superblock
    fprintf(fp, "Superblock\n");
    fprintf(fp, "Magic number: %d\n", file_system.superblock.magicNumber);
    fprintf(fp, "Block count: %d\n", file_system.superblock.blockCount);
    fprintf(fp, "Block size: %d\n", file_system.superblock.blockSize);
    fprintf(fp, "Directory block count: %d\n", file_system.superblock.directoryBlockCount);
    fprintf(fp, "Directory count: %d\n", file_system.superblock.directoryCount);
    fprintf(fp, "I-Node count: %d\n", file_system.superblock.iNodeBlockCount);
    fprintf(fp, "Start position of root directory: %d\n", file_system.superblock.startOfRootDirectory);
    fprintf(fp, "Start position of free space block: %d\n", file_system.superblock.startOfFreeSpace);
    fprintf(fp, "Start position of data blocks: %d\n", file_system.superblock.startOfDataBlock);
    fprintf(fp, "Start position of I-Node blocks: %d\n", file_system.superblock.startOfiNodeBlock);
    fprintf(fp, "Start position of directory blocks: %d\n", file_system.superblock.startOfDirectoryBlock);
    // Exporting root directory
    fprintf(fp, "\nRoot Directory\n");
    fprintf(fp, "Data block 1: %d\n", file_system.rootDirectoryBlock.dataBlocks[0]);
    fprintf(fp, "Data block 2: %d\n", file_system.rootDirectoryBlock.dataBlocks[1]);
    fprintf(fp, "Directory block 1: %d\n", file_system.rootDirectoryBlock.directoryBlocks[0]);
    fprintf(fp, "Directory block 2: %d\n", file_system.rootDirectoryBlock.directoryBlocks[1]);
    // Exporting free space block
    fprintf(fp, "\nFree Space Block\n");
    fprintf(fp, "File count at root: %d\n", file_system.freeSpaceBlock.fileCountAtRootDirectory);
    fprintf(fp, "Directory count at root: %d\n", file_system.freeSpaceBlock.directoryCountAtRootDirectory);
    fprintf(fp, "Number of free blocks: %d\n", file_system.freeSpaceBlock.numberOfFreeBlocks);
    fprintf(fp, "Number of free I-nodes: %d\n", file_system.freeSpaceBlock.numberOfFreeINodeBlocks);
    fprintf(fp, "Number of free directory block: %d\n", file_system.freeSpaceBlock.numberOfFreeDirectoryBlocks);
    fprintf(fp, "Number of free data block: %d\n", file_system.freeSpaceBlock.numberOfFreeDataBlocks);
    fprintf(fp, "First free data block: %d\n", file_system.freeSpaceBlock.firstFreeDataBlock);
    fprintf(fp, "First free directory block: %d\n", file_system.freeSpaceBlock.firstFreeDirectoryBlock);
    fprintf(fp, "First free I-node block: %d\n", file_system.freeSpaceBlock.firstFreeINodeBlock);
    // Exporting the I-node blocks
    fprintf(fp, "\nI-Nodes\n");
    for(int i=0; i<file_system.superblock.iNodeBlockCount; i++) {
        fprintf(fp, "Index: %d\n", i+1);
        fprintf(fp, "File size: %d\n", 0);
        fprintf(fp, "File name: %s\n", "\"\"");
        fprintf(fp, "Last modification: %s\n", "\"\"");
        fprintf(fp, "First Data Block: %d\n", 0+(6*i));
        fprintf(fp, "Second Data Block: %d\n", 1+(6*i));
        fprintf(fp, "Third Data Block: %d\n", 2+(6*i));
        fprintf(fp, "Single Indirect Data Block: %d\n", 3+(6*i));
        fprintf(fp, "Double Indirect Data Block: %d\n", 4+(6*i));
        fprintf(fp, "Triple Indirect Data Block: %d\n", 5+(6*i));
    }
    // Exporting the Directory Blocks
    fprintf(fp, "\nDirectory Blocks\n");
    for(int i=0; i<file_system.superblock.directoryBlockCount; i++) {
        fprintf(fp, "Index: %d\n", i+1);
        fprintf(fp, "Directory name: %s\n", "\"\"");
        fprintf(fp, "Data: %s\n", "\"\"");
        fprintf(fp, "Child Directory Block 1: %d\n", 0+(2*i));
        fprintf(fp, "Child Directory Block 2: %d\n", 1+(2*i));
    }
    // Exporting the Data Blocks
    fprintf(fp, "\nData Blocks\n");
    for(int i=0; i<file_system.freeSpaceBlock.numberOfFreeDataBlocks; i++) {
        fprintf(fp, "Index: %d\n", i+1);
        fprintf(fp, "Data: %s\n", "\"\"");
    }

    // Closing the file
    fclose(fp);
}

void exitWithError(char * errorMessage) {
    perror(errorMessage);
    exit(EXIT_FAILURE);
}