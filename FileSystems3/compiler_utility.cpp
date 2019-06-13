#include "stdafx.h"
#include "CompilerUtility.h"
#include <string.h>

int findChar(char *stng, char req){
	int i;
	for (i = 0; stng[i] != '\0'; i++)
		if (stng[i] == req)
			return i;
	return -1;
}

void removeNewLine(char *token){
	if (!token)
		return;
	if (token[strlen(token) - 1] == '\n')
		token[strlen(token) - 1] = '\0';
	if (token[strlen(token) - 1] == '\r')
		token[strlen(token) - 1] = '\0';
}

int getRegisterCode(char* reg){
	if (!strcmp(reg, "AX"))
		return AX;
	if (!strcmp(reg, "BX"))
		return BX;
	if (!strcmp(reg, "CX"))
		return CX;
	if (!strcmp(reg, "DX"))
		return DX;
	if (!strcmp(reg, "EX"))
		return EX;
	if (!strcmp(reg, "FX"))
		return FX;
	if (!strcmp(reg, "GX"))
		return GX;
	if (!strcmp(reg, "HX"))
		return HX;
	return -1;
}

int getOpcode(char *command){
	if (!strcmp("DATA", command))
		return DATA;
	if (!strcmp("MOV1", command))
		return MOV1;
	if (!strcmp("MOV2", command))
		return MOV2;
	if (!strcmp("ADD", command))
		return ADD;
	if (!strcmp("SUB", command))
		return SUB;
	if (!strcmp("MUL", command))
		return MUL;
	if (!strcmp("JMP", command))
		return JMP;
	if (!strcmp("IF", command))
		return IF;
	if (!strcmp("EQ", command))
		return EQ;
	if (!strcmp("LT", command))
		return LT;
	if (!strcmp("GT", command))
		return GT;
	if (!strcmp("LTEQ", command))
		return LTEQ;
	if (!strcmp("GTEQ", command))
		return GTEQ;
	if (!strcmp("PRINT", command))
		return PRINT;
	if (!strcmp("READ", command))
		return READ;
	if (!strcmp("CONST", command))
		return CONST;
	if (!strcmp("ENDIF", command))
		return ENDIF;
	if (findChar(command, ':') != -1)
		return LABEL;
	return -1;
}

int getSymbolTableIndex(compiler_metaData *meta_data, char entry){
	int i;
	for (i = 0; i < (*meta_data).symbolIndex; i++)
		if ((*meta_data).symbolTable[i].entry == entry)
			return (*meta_data).symbolTable[i].address;
	return -1;
}

void addLabelEntry(compiler_metaData *meta_data, char *label, int nextCommandNum){
	int index = (*meta_data).labelIndex;
	strcpy((*meta_data).labelTable[index].labelName, label);
	(*meta_data).labelTable[index].address = nextCommandNum;
	(*meta_data).labelIndex += 1;
}

void addSymbolEntry(compiler_metaData* meta_data, char name, int size){
	int index = (*meta_data).symbolIndex;
	int address = (*meta_data).symbolAddress;
	(*meta_data).symbolTable[index].entry = name;
	(*meta_data).symbolTable[index].address = address;
	(*meta_data).symbolTable[index].size = size;
	(*meta_data).symbolAddress += size;
	(*meta_data).symbolIndex += 1;
}

void addOpCode(code generatedCode[], int instructionNum, int opcode, int arg1, int arg2, int arg3, int jump, int index){

	generatedCode[index].instructionNumber = instructionNum;
	generatedCode[index].opCode = opcode;
	generatedCode[index].arg1 = arg1;
	generatedCode[index].arg2 = arg2;
	generatedCode[index].arg3 = arg3;
	generatedCode[index].jump = jump;
}
