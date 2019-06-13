#ifndef COMPILERSTRUCTURES_H
#define COMPILERSTRUCTURES_H

#define maxEntries 500
#define maxLabelLength 100
typedef struct symbolEntry symbolEntry;
typedef struct code code;
typedef struct mempry memory;
typedef struct label;
typedef struct compiler_metaData compiler_metaData;
typedef struct jumpStack jumpStack;

enum registers{ AX, BX, CX, DX, EX, FX, GX, HX };
enum commands{ DATA, MOV1, MOV2, ADD, SUB, MUL, JMP, IF, EQ, LT, GT, LTEQ, GTEQ, PRINT, READ, CONST, ENDIF, LABEL };


struct symbolEntry{
	char entry;
	int address, size;
};

struct code{
	int instructionNumber;
	int opCode;
	int arg1, arg2, arg3, jump;
};

struct memoryTable{
	char entry;
	int value;
};

struct label{
	char labelName[maxLabelLength];
	int address;
};

struct jumpStack{
	int commandNum;
	char location[maxLabelLength];
};

struct compiler_metaData{
	int labelIndex, memoryIndex, symbolIndex, symbolAddress, jumpStackTop;
	jumpStack jump_stack[maxEntries];
	memoryTable memory_table[maxEntries];
	label labelTable[maxEntries];
	symbolEntry symbolTable[maxEntries];
};
#endif