#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include "CompilerStructures.h"
#include "CompilerUtility.h"
#include "FileIO.h"
#include "compiler.h"
#define _CRT_SECURE_NO_WARNINGS

/*
Output Format:
Code
<new Line>
Symbol Table
<new Line>
Memory Table (for constants)
<new Line>

*/


void addMemoryEntry(compiler_metaData *meta_data, char entry, int value){
	int index = (*meta_data).memoryIndex;
	(*meta_data).memory_table[index].entry = entry;
	(*meta_data).memory_table[index].value = value;
	(*meta_data).memoryIndex += 1;
}

void addJumpStackEntry(compiler_metaData *meta_data, int commandNum, char *location){
	int index = ++(*meta_data).jumpStackTop;
	(*meta_data).jump_stack[index].commandNum = commandNum;
	strcpy((*meta_data).jump_stack[index].location, location);
}

int findLabelAddress(compiler_metaData *meta_data, char *location){
	int i;
	for (i = 0; i < (*meta_data).labelIndex; i++){
		if (!strcmp(location, (*meta_data).labelTable[i].labelName))
			return (*meta_data).labelTable[i].address;
	}
	return -1;
}

void parseAndStore(char* input, int *commandNum, code generatedCode[], compiler_metaData *meta_data){
	char *token = strtok(input, " "), *arg1 = " ", *arg2 = " ", *arg3 = " ",*temp;
	int opCode, regCode1, regCode2, regCode3, start, end, size;
	temp = (char*)malloc(50 * sizeof(char));
	if (!strcmp("MOV", token)){
		arg1 = strtok(NULL, " ");
		arg1 = strtok(arg1, ",");
		arg2 = strtok(NULL, ",");
		removeNewLine(arg2);
		regCode1 = getRegisterCode(arg1);
		regCode2 = getRegisterCode(arg2);
		if (regCode2 != -1 && regCode1 != -1){
			//Register to register. Not in specs.
		}
		else if (regCode2 != -1){
			//temp = 
			strcpy(temp, token);
			strcat(temp, "1");
			token = temp;
		}
		else if (regCode1 != -1){
			strcpy(temp, token);
			strcat(temp, "2");
			token = temp;
		}
		else{
			//Mem to Mem, Not possible
		}
	}

	opCode = getOpcode(token);
	switch (opCode){
		case DATA:
			arg1 = strtok(NULL, " ");
			removeNewLine(arg1);
			start = findChar(arg1, '[');
			end = findChar(arg1, ']');
			if (start == -1)
				size = 1;
			else{
				strncpy(temp, &arg1[start + 1], end - start - 1);
				temp[end - start - 1] = '\0';
				size = atoi(temp);
			}
			addSymbolEntry(meta_data, arg1[0], size);
			break;
		case MOV1:
			addOpCode(generatedCode, *commandNum, opCode, getSymbolTableIndex(meta_data, arg1[0]), regCode2, -1, -1, *commandNum - 1);
			(*commandNum) += 1;
			break;
		case MOV2:
			addOpCode(generatedCode, *commandNum, opCode, regCode1, getSymbolTableIndex(meta_data, arg2[0]), -1, -1, *commandNum - 1);
			(*commandNum) += 1;
			break;
		case ADD:
		case SUB:
		case MUL:
			arg1 = strtok(NULL, " ");
			arg1 = strtok(arg1, ",");
			arg2 = strtok(NULL, ",");
			arg3 = strtok(NULL, ",");
			removeNewLine(arg3);
			regCode1 = getRegisterCode(arg1);
			regCode2 = getRegisterCode(arg2);
			regCode3 = getRegisterCode(arg3);
			if (regCode2 != -1 && regCode3 != -1)
				addOpCode(generatedCode, *commandNum, opCode, regCode1, regCode2, regCode3, -1, *commandNum - 1);
			else if (regCode2 != -1)
				addOpCode(generatedCode, *commandNum, opCode, regCode1, regCode2, getSymbolTableIndex(meta_data, arg3[0]), -1, *commandNum - 1);
			else
				addOpCode(generatedCode, *commandNum, opCode, regCode1, getSymbolTableIndex(meta_data, arg2[0]), regCode3, -1, *commandNum - 1);
			(*commandNum) += 1;
			break;
		case PRINT:
			arg1 = strtok(NULL, " ");
			removeNewLine(arg1);
			addOpCode(generatedCode, *commandNum, opCode, getSymbolTableIndex(meta_data, arg1[0]), -1, -1, -1, *commandNum - 1);
			break;
		case READ:
			arg1 = strtok(NULL, " ");
			removeNewLine(arg1);
			regCode1 = getRegisterCode(arg1);
			addOpCode(generatedCode, *commandNum, opCode, regCode1, -1, -1, -1, *commandNum - 1);
			(*commandNum) += 1;
			break;
		case LABEL:
			removeNewLine(token);
			addLabelEntry(meta_data, token, (*commandNum) + 1);
			break;
		case CONST:
			arg1 = strtok(NULL, " ");
			arg1 = strtok(arg1, "=");
			arg2 = strtok(NULL, "=");
			addMemoryEntry(meta_data, arg1[0], atoi(arg2));
			addSymbolEntry(meta_data, arg1[0], 0);
			break;
		case JMP:
			arg1 = strtok(NULL, " ");
			addJumpStackEntry(meta_data, *commandNum, arg1);
			//(*meta_data).jumpStack[++(*meta_data).jumpStackTop] = (*commandNum);
			addOpCode(generatedCode, *commandNum, opCode, -1, -1, -1, -1, (*commandNum) - 1);
			break;
	}

}

void parseIf(char *buffer, code generatedCode[], int *commandNum, int stack[], int *top){
	char *token = strtok(buffer, " "), *operand1, *operation, *operand2, popped, prev = -1;
	removeNewLine(token);
	if (!strcmp(token, "IF")){
		(*top) += 1;
		operand1 = strtok(NULL, " ");
		operation = strtok(NULL, " ");
		operand2 = strtok(NULL, " ");
		stack[(*top)] = *commandNum;
		addOpCode(generatedCode, *commandNum, IF, getRegisterCode(operand1), getRegisterCode(operand2), getOpcode(operation), -1, (*commandNum) - 1);
		(*commandNum) += 1;
	}
	else if (!strcmp(token, "ELSE")){
		(*top) += 1;
		stack[(*top)] = *commandNum;
		addOpCode(generatedCode, *commandNum, JMP, -1, -1, -1, -1, (*commandNum) - 1);
		(*commandNum) += 1;
	}
	else if (!strcmp(token, "ENDIF")){
		popped = stack[(*top)];
		(*top) -= 1;
		if (generatedCode[popped - 1].opCode == IF)
			generatedCode[popped - 1].jump = *commandNum;
		else{
			generatedCode[popped - 1].jump = *commandNum;
			prev = generatedCode[popped - 1].instructionNumber;
			popped = stack[(*top)];
			(*top) -= 1;
			generatedCode[popped - 1].jump = prev + 1;
		}
	}
}

void writeOutput(int commandNum, code generatedCode[], compiler_metaData meta_data){
	int i, symbolTableEntries, memoryTableEntries;
	FILE *output = fopen("output.txt", "w");
	char *buffer;
	
	symbolTableEntries = meta_data.symbolIndex;
	memoryTableEntries = meta_data.memoryIndex;
	buffer = (char*)malloc(33 * commandNum + 12 * symbolTableEntries + 7 * memoryTableEntries + 1);
	
	for (i = 0; i < commandNum; i++){
		sprintf(buffer, "%d) %d ", generatedCode[i].instructionNumber, generatedCode[i].opCode);
		if (generatedCode[i].arg1 != -1)
			sprintf(buffer, "%d ", generatedCode[i].arg1);
		if (generatedCode[i].arg2 != -1)
			sprintf(buffer, "%d ", generatedCode[i].arg2);
		if (generatedCode[i].arg3 != -1)
			sprintf(buffer, "%d ", generatedCode[i].arg3);
		if (generatedCode[i].jump != -1)
			sprintf(buffer, "%d ", generatedCode[i].jump);
		sprintf(buffer, "\n");
	}
	sprintf(buffer, "\n");

	for (i = 0; i < symbolTableEntries; i++){
		sprintf(buffer, "%c %d %d\n", meta_data.symbolTable[i].entry, meta_data.symbolTable[i].address, meta_data.symbolTable[i].size);
	}
	sprintf(buffer, "\n");

	for (i = 0; i < memoryTableEntries; i++){
		sprintf(buffer, "%c %d\n", meta_data.memory_table[i].entry, meta_data.memory_table[i].value);
	}
	sprintf(buffer, "\n");
	printf("check %s\n", buffer);
	
	fclose(output);
}

void takeInput(char *fileName){
	//FILE *input = fopen(fileName, "r");
	char buffer[500], tempBuff[500],*input;
	int comNum = 1, i, stackTop, index,offset;
	compiler_metaData meta_data;
	code generatedCode[maxEntries];
	char *token;
	int stack[maxEntries], top = -1;

	if (!fileName){
		printf("No input file found\n");
		return;
	}

	meta_data.labelIndex = 0;
	meta_data.memoryIndex = 0;
	meta_data.symbolIndex = 0;
	meta_data.symbolAddress = 0;
	meta_data.jumpStackTop = -1;

	//if (!input){
	//	printf("Input file does not exist\n");
	//	return;
	//}

	input = getFile(fileName, NULL, 1);
	while (sscanf(input, "%[^\n]s%n", buffer,&offset) == 1){
		i = 0;
		input += offset;
		printf("check scanned %s\n", buffer);
		while (buffer[i] == ' ')
			i++;
		removeNewLine(buffer);
		strcpy(buffer, &buffer[i]);
		strcpy(tempBuff, buffer);
		token = strtok(tempBuff, " ");
		removeNewLine(token);
		if (!strcmp("IF", token) || !strcmp("ELSE", token) || !strcmp("ENDIF", token))
			parseIf(buffer, generatedCode, &comNum, stack, &top); //Processing Conditional operators using stack
		else
			parseAndStore(buffer, &comNum, generatedCode, &meta_data); //Processing other operators
	}

	//Add all jump destinations
	while (meta_data.jumpStackTop > -1){
		stackTop = meta_data.jumpStackTop;
		//meta_data.jump_stack[0].
		index = meta_data.jump_stack[stackTop].commandNum;
		generatedCode[index].arg1 = findLabelAddress(&meta_data, meta_data.jump_stack[stackTop].location);
		meta_data.jumpStackTop--;
	}

	writeOutput(comNum, generatedCode, meta_data);

	//fclose(input);
}