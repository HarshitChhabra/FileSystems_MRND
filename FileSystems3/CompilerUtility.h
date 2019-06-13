#ifndef COMPILERUTILITY_H
#define COMPILERUTILITY_H
#include "CompilerStructures.h"

void addOpCode(code generatedCode[], int instructionNum, int opcode, int arg1, int arg2, int arg3, int jump, int index);
void addSymbolEntry(compiler_metaData* meta_data, char name, int size);
void addLabelEntry(compiler_metaData *meta_data, char *label, int nextCommandNum); 
int getSymbolTableIndex(compiler_metaData *meta_data, char entry);
int getOpcode(char *command);
int getRegisterCode(char* reg);
void removeNewLine(char *token);
int findChar(char *stng, char req);

#endif