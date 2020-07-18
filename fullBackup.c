#include "fullBackup.h"
#define MAX_NAME_LENGTH 100
#define LINE_NUMBERS    200
#define LINE_LENGTH     MAX_NAME_LENGTH


/**
 * This function is make a fullbackup of the file that given to it 
 * for example the file name is 'filename.txt' and it make a backup named 'filename.cab' 
 * 
 * @param fileName: filename that given to make full backup is somthing like "filename.txt"
 * @param dirPath : the path where the file exists . 
 * 
 * @return        :
 */
void fullBackup(char* fileName, char *dirPath){
    
    const char s[2] = ".";
    char *fileNameWithoutExtension;

    char fileNameCopy[MAX_NAME_LENGTH];
    strcpy(fileNameCopy, fileName);

    fileNameWithoutExtension = strtok(fileNameCopy, s);//for example we have filename.txt but we just want 'filename'
    
    char *fnwe = getfileNameWithoutExtention(fileName);
    char fileNameWithoutExtention[MAX_NAME_LENGTH];
    strcpy(fileNameWithoutExtention, fnwe);

    char *ex = getFileExtention(fileName);
    char extention[MAX_NAME_LENGTH];
    strcpy(extention, ex);

    char command[MAX_NAME_LENGTH * 3];
    sprintf(command, "mkdir \"%s\\log\"",dirPath);
    system(command);

    char log[MAX_NAME_LENGTH * 3];
    sprintf(log,"%s\\log\\log.txt",dirPath);
    FILE *logger = fopen(log, "a");

    

    //compressing the file with .cab format 
    sprintf(command,"makecab \"%s\\%s\" \"%s\\%s.cab\"", dirPath, fileName, dirPath, fileNameWithoutExtension);
    system(command);//"makecab filename.txt filename.cab"

    fputs("FULL BACKUP:\n", logger);
    sprintf(log, "\tCOMPRESS\t '%s' AS '%s.cab'\n", fileName, fileNameWithoutExtension);
    fputs(log, logger);

    //copying the file to have the last version
    sprintf(command, "copy \"%s\\%s\" \"%s\\%s_n-1%s\"", dirPath, fileName, dirPath, fileNameWithoutExtention,extention);
    system(command);//copy filename.txt filename_n-1.txt

    fclose(logger);
}



/**
 * this function will return the extention of the file that given to it .
 * 
 * @param          fileName        : filename that given to get its extention('.abc') .
 * @return fileNameWithoutExtension: the extention(.abc) of the file given .
 */
char *getfileNameWithoutExtention(char *fileName){
    const char s[2] = ".";
    char fileNameCopy[MAX_NAME_LENGTH];
    strcpy(fileNameCopy, fileName);
    //printf("filenameCPY, filename: %s %s\n", fileNameCopy, fileName);
    /* get the first token */
    char *fileNameWithoutExtension;
    fileNameWithoutExtension=strtok(fileNameCopy, s);
    return fileNameWithoutExtension;
}