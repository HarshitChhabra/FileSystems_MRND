#ifndef COMPILER_H
#define COMPILER_H
#include "CompilerStructures.h"
void addMemoryEntry(compiler_metaData *meta_data, char entry, int value);
void addJumpStackEntry(compiler_metaData *meta_data, int commandNum, char *location);
int findLabelAddress(compiler_metaData *meta_data, char *location);
void parseAndStore(char* input, int *commandNum, code generatedCode[], compiler_metaData *meta_data);
void parseIf(char *buffer, code generatedCode[], int *commandNum, int stack[], int *top);
void writeOutput(int commandNum, code generatedCode[], compiler_metaData meta_data);
void takeInput(char *fileName);
#endif