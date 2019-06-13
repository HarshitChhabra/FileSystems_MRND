#ifndef FILEIO_H
#define FILEIO_H
#include "DiskIO.h"

metaData* getMetaData();
void setMetaData(metaData* meta);
void enterFile(char *fileName, char *outputFile);
char* getFile(char *fileName, char *output,int compilerFlag);
void format(int block);
void deleteFile(char *fileName);
void debug();
void listAllFiles();
void initFileIO(char *hddName, int block);
#endif