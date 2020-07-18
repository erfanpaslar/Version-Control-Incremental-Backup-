#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>

struct Array2d gettingAllVersionOfFile(char *fileNameWithoutExtention, char *pathToGetSubdirectories);
void restore(char *fileNewName, char *dirPath, int version);
char *getfileNameWithoutExtention(char *fileName);//should i declear it twice??
char *getFileExtention(char *filename);
int isFileNameExists(struct Array2d direc, char filenameToRE[]);
int isStrInStr(char string[], char subString[]);