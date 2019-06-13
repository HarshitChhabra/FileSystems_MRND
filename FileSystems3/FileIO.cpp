#include "stdafx.h"
#include "FileIO.h"
#include <stdlib.h>
#include <string.h>
//#define blockSize 16384
//#define maxBlocks 100

static int blockSize;

metaData* getMetaData(){
	
	int metaSize = sizeof(metaData);
	int i=0;
	//int blocks = (100 * 1024 * 1024) / blockSize,i=0;
	//int metaSize = sizeof(metaData) + (blocks*sizeof(bool)) - (2 * sizeof(bool));
	//metaData *meta = (metaData*)malloc(sizeof(metaData) + (blocks*sizeof(bool)) - (2 * sizeof(bool)));
	metaData *meta = (metaData*)malloc(sizeof(metaData));
	metaData *original = meta;
	//void *metaItr =meta;
	while (metaSize > blockSize){
		memcpy(meta, getBlock(i), blockSize);
		i++;
		meta += (i*blockSize);
		metaSize -= blockSize;
	}
	memcpy(meta, getBlock(i), metaSize);
	return original;
}

void setMetaData(metaData* meta){
	//int blocks = (100 * 1024 * 1024) / blockSize,i=0;
	//int metaSize = sizeof(metaData) + (blocks*sizeof(bool)) - (2 * sizeof(bool));
	int i=0,metaSize = sizeof(metaData);
	void *buffer = (void*)malloc(blockSize);
	while (metaSize > blockSize){
		memcpy(buffer, meta, blockSize);
		setBlock(i, buffer);
		i++;
		meta += (i*blockSize);
		metaSize -= blockSize;
	}
	memcpy(buffer, meta, metaSize);
	setBlock(i, buffer);
	//i++;
	//meta += (i*blockSize);
	//metaSize -= blockSize;
	//setBlock(0, meta);
}

int getNextBlockNum(bool *arr){
	int i;
	for (i = 0; i < 6400; i++){//TO BE CHANGED
		if (arr[i] == 1)
			return i;
	}
	return 0;
}

void enterFile(char *fileName, char *outputName){
	int size, blocksRequired, i, blockNum, *blocksOccupied;
	FILE *toBeWrittenFile = fopen(fileName, "rb");
	file *inputFile = (file*)malloc(sizeof(file));
	char* buffer = (char*)malloc(blockSize*sizeof(char));
	metaData *meta = (metaData*)malloc(sizeof(metaData));
	if (!toBeWrittenFile){
		printf("Cannot find %s\n", fileName);
		exit(0);
	}
	fseek(toBeWrittenFile, 0L, SEEK_END);
	size = ftell(toBeWrittenFile);
	blocksRequired = size / blockSize;
	if (size%blockSize != 0)
		blocksRequired++;
	strcpy(inputFile->fileName, outputName);
	inputFile->fileLength = size;
	inputFile->numOfBlocks = blocksRequired;

	meta = getMetaData();
	if (meta->magic != 9){
		printf("HDD corrupt\n");
		return;
	}
	if (meta->freeBlocks < blocksRequired){
		printf("Memory not available\n");
		return;
	}
	printf("Blocks required %d\n", blocksRequired);
	blocksOccupied = (int*)malloc((blocksRequired)*sizeof(int));

	for (i = 0; i < 6400; i++)
		if (meta->blocks[i])
			break;

	if (blocksRequired == 1)
		inputFile->startBlock = i;

	fseek(toBeWrittenFile, 0, SEEK_SET);

	for (i = 0; i < blocksRequired; i++){
		blockNum = getNextBlockNum(meta->blocks);
		blocksOccupied[i] = blockNum;
		printf("Setting in %d\n", blockNum);
		fread(buffer, blockSize, 1, toBeWrittenFile);

		setBlock(blockNum, buffer);
		meta->blocks[blockNum] = 0;
	}
	if (blocksRequired > 1){
		blockNum = getNextBlockNum(meta->blocks);
		printf("Setting record in %d\n", blockNum);
		setBlock(blockNum, blocksOccupied);
		meta->blocks[blockNum] = 0;
		inputFile->startBlock = blockNum;
	}

	meta->files[meta->numOfFiles] = *inputFile;
	meta->freeBlocks -= blocksRequired;
	meta->numOfFiles += 1;
	setMetaData(meta);

	fclose(toBeWrittenFile);
}

char* getFile(char *fileName, char *outputName,int compilerFlag){
	FILE *outputFile=NULL;
	int startBlock, numOfBlocks, fileLength, metaSize = sizeof(metaData), i, *blocksOccupied;
	metaData *meta;
	file *tempFile;
	char *buffer,*output,*tempOutput;
	if (outputName)
		outputFile= fopen(outputName, "wb");
	meta = getMetaData();
	if (meta->magic != 9){
		printf("HDD corrupt\n");
		exit(0);
	}
	for (i = 0; i < meta->numOfFiles; i++){
		printf("File %s %d\n", meta->files[i].fileName, meta->numOfFiles);
		if (!strcmp(meta->files[i].fileName, fileName)){
			startBlock = meta->files[i].startBlock;
			numOfBlocks = meta->files[i].numOfBlocks;
			fileLength = meta->files[i].fileLength;
			printf("Found %d %d\n", startBlock, numOfBlocks);
			break;
		}
	}
	if (i == meta->numOfFiles){
		printf("File %s not found\n", fileName);
		return NULL;
	}
	output = (char*)malloc(fileLength);
	tempOutput = output;
	if (numOfBlocks == 1){
		
		buffer = (char*)getBlock(startBlock);
		strcpy(output, buffer);
		if (!compilerFlag)
			fwrite(buffer, fileLength, 1, outputFile);
	}
	else{
		blocksOccupied = (int*)malloc(numOfBlocks*sizeof(int));
		memcpy(blocksOccupied, getBlock(startBlock), numOfBlocks*sizeof(int));
		for (i = 0; i < numOfBlocks; i++){
			if (fileLength > blockSize){
				printf("Getting from %d\n", blocksOccupied[i]);
				buffer = (char*)getBlock(blocksOccupied[i]);
				strcpy(tempOutput, buffer);
				if (!compilerFlag)
					fwrite(buffer, blockSize, 1, outputFile);
				fileLength -= blockSize;
				tempOutput += blockSize;
			}
			else{
				buffer = (char*)getBlock(blocksOccupied[i]);
				strcpy(tempOutput, buffer);
				if (!compilerFlag)
					fwrite(buffer, fileLength, 1, outputFile);
			}
		}
	}
	if (outputFile)
		fclose(outputFile);
	return output;
	
}

void format(int block){
	//int i,blocks = (100*1024*1024)/block;
	int i, blocks = 6400;
	metaData *meta = (metaData*)malloc(sizeof(metaData));
	meta->magic = 9;
	meta->numOfFiles = 0;
	//meta->blocks = (bool*)malloc(blocks*sizeof(bool));
	for (i = 0; i < blocks; i++)
		meta->blocks[i] = 1;
	meta->blocks[0] = 0;
	meta->freeBlocks = blocks;
	//setBlock(0, meta);
	setMetaData(meta);
}

void deleteFile(char *fileName){
	int blocksRequired = 0, metaSize = sizeof(metaData), i,*blocksOccupied,j;
	FILE *toBeWrittenFile = fopen(fileName, "r");
	FILE *hdd;
	file *inputFile = (file*)malloc(sizeof(file));
	long int size;
	metaData *meta;
	meta = getMetaData();
	if (meta->magic != 9){
		printf("HDD corrupted\n");
		return;
	}
	for (i = 0; i < meta->numOfFiles; i++){
		if (!strcmp(meta->files[i].fileName, fileName)){
			blocksRequired = meta->files[i].numOfBlocks;
			if (blocksRequired == 1)
				meta->blocks[meta->files[i].startBlock] = 1;
			else{
				blocksOccupied = (int*)malloc(blocksRequired*sizeof(int));
				memcpy(blocksOccupied,getBlock(meta->files[i].startBlock),blocksRequired*sizeof(int));
				for (j = 0; j < blocksRequired; j++)
					meta->blocks[blocksOccupied[j]] = 1;	
			}
			meta->files[i] = meta->files[meta->numOfFiles - 1];
			meta->numOfFiles -= 1;
			setMetaData(meta);
			printf("Deleted\n");
			return;
		}
	}
	printf("File %s not found\n", fileName);
	return;
}

void debug(){
	int i;
	metaData *meta = getMetaData();
	if (meta->magic != 9){
		printf("HDD Corrupted\n");
		return;
	}
	printf("Number of files: %d\n", meta->numOfFiles);
	printf("Number of blocks available %d\n", meta->freeBlocks);
	printf("Number of files in disk %d\n", meta->numOfFiles);
	printf("Each block size: %d\n", blockSize);
	for (i = 0; i < meta->numOfFiles; i++)
		printf("File %d: %s\n", i + 1, meta->files[i].fileName);
	
}

void listAllFiles(){
	int i;
	metaData *meta = getMetaData();
	if (meta->magic != 9){
		printf("HDD Corrupted\n");
		return;
	}
	//printf("%d\n", meta->numOfFiles);
	for (i = 0; i < meta->numOfFiles; i++)
		printf("File %d: %s\n", i + 1, meta->files[i].fileName);
}

void initFileIO(char *hddName, int block){
	metaData *meta;
	unsigned int numOfBlocks;
	numOfBlocks = init(hddName, block);
	if (!numOfBlocks){
		printf("error\n");
		return;
	}
		//return;
	blockSize = block;
	printf("got block %d\n", block);
	//meta = (metaData*)getBlock(0);
	meta = getMetaData();
	if (meta->magic != 9){
		printf("Corrupted or new hard disk found! Formatting now..\n");
		format(block);
	}

}