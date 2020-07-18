#include "restore.h"

#define MAX_NAME_LENGTH 100
#define LINE_NUMBERS    200
#define LINE_LENGTH     MAX_NAME_LENGTH

/**
 * this will help us return an array of intigers.
 */ 
typedef struct Array2d{
    char array[LINE_NUMBERS][LINE_LENGTH];
    int  lines;
};


/**
 * This function will make the previous version with the current version and inc.txt 
 *                with incrementalBackup function
 *                            ^
 *  old         current       |         inc/diff        restore
 *  file1.txt   file2.txt ----------->  inc.txt         restore.txt
 *  p1          p1                      .               p1
 *  p2          p1.5                    +p1.5           p2
 *  p3          p2                      .               p3
 *                                      -p3
 *  
 * as you see we make the inc.txt with the incrementalBackup function and this function \
 * will create the old version that we dont have now 
 *
 * @param fileNewName: name of the file that we want to restor one of its version .
 * @param     dirPath:      the path to the directory that the file exists        .
 * @param     version:        versin of the file that we want to restore          .
 * 
 * @return
 */
void restore(char *fileNewName, char *dirPath, int version){

    char fileName[MAX_NAME_LENGTH];
    char copy[MAX_NAME_LENGTH];
    strcpy(copy, fileNewName);
    
    char *ex = getFileExtention(copy);
    char extention[MAX_NAME_LENGTH];
    strcpy(extention, ex);

    strcpy(copy, fileNewName);
    char *fnwe = getfileNameWithoutExtention(copy);//this function exists in fullBackup.c
    char fileNameWithoutExtention[MAX_NAME_LENGTH];
    strcpy(fileNameWithoutExtention, fnwe);

    char fileLogName[MAX_NAME_LENGTH];

    char fileRestoreName[MAX_NAME_LENGTH];
    sprintf(fileRestoreName, "\"%s\\%s_restoredV%d%s\"", dirPath, fileNameWithoutExtention, version, extention);
    sprintf(fileName, "\"%s\\%s\"", dirPath, fileNewName);

    

    char stringLines[MAX_NAME_LENGTH+1];
    int  lineNumber = 0;

    struct Array2d allVersions = gettingAllVersionOfFile(fileNameWithoutExtention, dirPath);
    int lastVersion = allVersions.lines-1;
    //for example our last version is '6'. we want version '4' \
     |_> first we should extract filename6.cab(inc6.txt) \
     |_> then with filename_n-1.txt and inc6.txt we make filename_temp5.txt \
     |_> then with inc5.txt and filename_temp5.txt we make filename_temp4.txt thats the version we want.
    char command[MAX_NAME_LENGTH*10];
    sprintf(command, "mkdir \"%s\\temp\"",dirPath);
    system(command);

    sprintf(command, "copy \"%s\\%s_n-1%s\" \"%s\\temp\\%s_temp%d%s\"",dirPath, fileNameWithoutExtention, extention,  dirPath,fileNameWithoutExtention,version,extention);
    system(command);

    // struct Array2d direc = GettinAllDirectories(dirPath);
    // if(!isFileNameExists(direc, "temp")){
    //     sprintf(command, "mkdir \"%s\\log\"",dirPath);
    //     system(command);
    // }
    sprintf(command, "mkdir \"%s\\log\"",dirPath);
        system(command);

    while (lastVersion>version){
        //extract last version to temp by name inc6.txt
        //restore versin 5 by restore(inc6)

        //unpack the .cab file
        sprintf(command, "expand \"%s\\%s%d.cab\" \"%s\\temp\\inc%d.txt\"", dirPath, fileNameWithoutExtention, lastVersion, dirPath, lastVersion);
        system(command); //expand path\\filename6.cab path\\temp\\inc6.txt 

        sprintf(fileLogName, "%s\\temp\\inc%d.txt", dirPath, lastVersion);
        sprintf(fileName, "%s\\temp\\%s_temp%d%s", dirPath, fileNameWithoutExtention, version, extention);
        sprintf(fileRestoreName, "%s\\temp\\%s_temp%d%s", dirPath, fileNameWithoutExtention, version - 1, extention);

        FILE *fileLog     = fopen(fileLogName    , "r");
        FILE *fileNew     = fopen(fileName       , "r");
        FILE *fileRestore = fopen(fileRestoreName, "w");

        if (fileRestore == NULL || fileNew == NULL || fileLog == NULL){ 
            printf("Error : Files not open"); 
            exit(0); 
        } 

        while (fgets(stringLines, 100, fileLog)){
            if (stringLines[0] == '.'){
                fgets(stringLines, 100, fileNew);
                fprintf(fileRestore, "%s", stringLines);
            }

            else if (stringLines[0] == '+'){ 
                fgets(stringLines, 100, fileNew);
            }

            else if (stringLines[0] == '-'){

                if (strlen(stringLines) == 1){
                    fprintf(fileRestore, "\n");
                }

                else{
                char copy[100];
                strcpy(copy, &stringLines[1]);
                fprintf(fileRestore,"%s",copy);
                }
            }
        }

        fclose(fileNew); 
        fclose(fileLog); 
        fclose(fileRestore); 

        lineNumber++;
        lastVersion--;
    }
    sprintf(command, "copy \"%s\\temp\\%s_temp%d%s\" \"%s\\%s_restoredV%d%s\"", dirPath, fileNameWithoutExtention, version - 1, extention, dirPath, fileNameWithoutExtention, version, extention);
    system(command);

    char log[MAX_NAME_LENGTH * 3];
    sprintf(log,"%s\\log\\log.txt",dirPath);
    FILE *logger = fopen(log, "a");

    sprintf(log, "RESTORE:\t\t version'%d' restored\n", version);
    fputs(log, logger);
    fclose(logger);

}


/**
 * This function is getting all names that matches the file name and put them in a 2d char array 
 * 
 * @param fileNameWithoutExtention : this is the file name without extention NOT 'filename.txt' BUT 'filename' .
 * @param  pathToGetSubdirectories : this is the path that we want to find the matches. 
 * 
 * @return       directories       : the same name directories in a 2d array .       
 */ 

struct Array2d gettingAllVersionOfFile(char *fileNameWithoutExtention,char *pathToGetSubdirectories){
    struct dirent *de;
    struct Array2d directories;
    directories.lines = 0;
    DIR *dr = opendir(pathToGetSubdirectories); 

	if (dr == NULL){ 
		printf("Could not open current directory" ); 
	}

    while ((de = readdir(dr)) != NULL) {

		if ( strcmp(de->d_name, ".")==0 || strcmp(de->d_name, "..")==0 )continue;

        char *is = strstr(de->d_name, fileNameWithoutExtention);
        char cab[10] = ".cab";

        if (is){

            if(isStrInStr(de->d_name, cab)){

                printf("%s\n", de->d_name);
                strcpy(directories.array[directories.lines], de->d_name);
                directories.lines++;

            }
        }
    }

	closedir(dr);
    return directories;

}


/**
 * This function is getting all names that matches the file name and put them in a 2d char array 
 * 
 * @param fileNameWithoutExtention : this is the file name without extention NOT 'filename.txt' BUT 'filename' .
 * @param  pathToGetSubdirectories : this is the path that we want to find the matches. 
 * 
 * @return       directories       : the same name directories in a 2d array .       
 */ 
char *getFileExtention(char *filename){
    char *ext = strrchr(filename, '.');
    if (ext)
        return(ext);
}


/**
 * This function is returns 1 if substring exists in string and 0 otherwise . 
 * 
 * @param  string   : string that we want to see if there is the substring exists in it .
 * @param subString : the sub string .
 * 
 * @return : returns 1 if substring exists in string and 0 otherwise .    
 */ 
int isStrInStr(char string[], char subString[]){

    char *ptr = strstr(string, subString);
    if(ptr){
        return 1;
    }
    return 0;
}