#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

struct Array2d GettinAllDirectories(char *pathToGetSubdirectories);
void backupTheFile(char *filename, char *dirPath);
int isStrInStr(char string[], char subString[]);
int isFileNameExists(struct Array2d direc, char filenameToRE[]);
char *getFileExtention(char *filename);
char *getfileNameWithoutExtention(char *fileName);//should i declear it twice??