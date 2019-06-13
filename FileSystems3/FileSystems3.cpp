// FileSystems2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "FileIO.h"
#include "compiler.h"
#define _CRT_SECURE_NO_WARNINGS

void toLowerStng(char *stng){
	int i;
	for (i = 0; stng[i] != '\0'; i++)
		stng[i] = tolower(stng[i]);
}

int getCommand(char *command){
	toLowerStng(command);
	if (!strcmp("mount", command))
		return 1;
	if (!strcmp("copytofs", command))
		return 2;
	if (!strcmp("copyfromfs", command))
		return 3;
	if (!strcmp("format", command))
		return 4;
	if (!strcmp("ls", command))
		return 5;
	if (!strcmp("delete", command))
		return 6;
	if (!strcmp("debug", command))
		return 7;
	if (!strcmp("exit", command))
		return 8;
	if (!strcmp("compile", command))
		return 9;
	return -1;
}

void start(){
	char inputCommand[1000],*token,*arg1,*arg2;
	int commandNum;
	while (1){
		fflush(stdin);
		printf("> ");
		scanf("%[^\n]s", inputCommand);
		token = strtok(inputCommand, " ");
		commandNum = getCommand(token);
		switch (commandNum){
			case 1:
				arg1 = strtok(NULL, " ");
				arg2 = strtok(NULL, " ");
				if (!arg1 || !arg2)
					printf("2 Arguements required\n");
				else{
					initFileIO(arg1, atoi(arg2));
					printf("Initialised\n");
				}
				break;
			case 2:
				arg1 = strtok(NULL, " ");
				arg2 = strtok(NULL, " ");
				if (!arg1 || !arg2)
					printf("2 Arguements required\n");
				else{
					enterFile(arg1, arg2);
					printf("File %s written\n", arg2);
				}
				break;
			case 3:
				arg1 = strtok(NULL, " ");
				arg2 = strtok(NULL, " ");
				if (!arg1 || !arg2)
					printf("2 Arguements required\n");
				else{
					getFile(arg1, arg2,0);
					printf("Created %s\n", arg2);
				}
				break;
			case 4:
				arg1 = strtok(NULL, " ");
				if (!arg1)
					printf("Block size required\n");
				else{
					format(atoi(arg1));
					printf("Disk formatted\n");
				}
				break;
			case 5:
				listAllFiles();
				break;
			case 6:
				arg1 = strtok(NULL, " ");
				if (!arg1)
					printf("File name required\n");
				else{
					deleteFile(arg1);
				}
				break;
			case 7:
				debug();
				break;
			case 8:
				exit(0);
				break;
			case 9:
				arg1 = strtok(NULL, " ");
				if (!arg1)
					printf("File name required\n");
				else if (arg1[strlen(arg1) - 1] == '\n')
					arg1[strlen(arg1) - 1] = '\0';
				else
					takeInput(arg1);
				break;
			default:
				printf("Wrong command\n");
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//format(0);
	//enterFile("hints1.txt");
	//getFile("hints1.txt", "yolo.txt");
	//printf("check %d\n", sizeof(struct student));
	start();
	//enterFile("timetable.png", "timetableInFs.png");
	//getFile("timetableInFs.png", "check_img_out_timetable.png");
	return 0;
}

