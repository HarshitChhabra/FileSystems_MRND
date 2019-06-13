#include "stdafx.h"

typedef struct metaData metaData;
typedef struct file file;

struct file{
	char fileName[30];
	unsigned int startBlock, numOfBlocks, fileLength;
};

struct metaData{
	int magic;
	file files[32];
	int numOfFiles;
	int freeBlocks;
	int numOfBlocks;
	bool blocks[6400];
	//bool *available_block;
};

void deleteFile(char *fileName);
void setBlock(int block, void *buffer);
void* getBlock(int block);
unsigned int init(char *hdd, int block);