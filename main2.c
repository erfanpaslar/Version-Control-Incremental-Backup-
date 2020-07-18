#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>


#define MAX_NAME_LENGTH 100
#define LINE_NUMBERS    200
#define LINE_LENGTH     MAX_NAME_LENGTH

char *getfileNameWithoutExtention(char *fileName);
char *getFileExtention(char *filename);
void fullBackup(char *fileName, char *dirPath);

struct Array2d GettinAllDirectories(char *pathToGetSubdirectories);
void backupTheFile(char *filename, char *dirPath);
int isStrInStr(char string[], char subString[]);
int isFileNameExists(struct Array2d direc, char filenameToRE[]);
char *getFileExtention(char *filename);
char *getfileNameWithoutExtention(char *fileName);//should i declear it twice??

struct Array2d gettingAllVersionOfFile(char *fileNameWithoutExtention, char *pathToGetSubdirectories);
void restore(char *fileNewName, char *dirPath, int version);
char *getfileNameWithoutExtention(char *fileName);//should i declear it twice??
char *getFileExtention(char *filename);
int isFileNameExists(struct Array2d direc, char filenameToRE[]);
int isStrInStr(char string[], char subString[]);

/**
 * this is the main function that gets option to do thing like:
 *     FullBackup     : make a full backup of a file that given and compress it
 * Incremental Backup : finds the different between current and last version of the file and compress the difference
 *       Restore      : after making some backups if we want to get the specific version we use this 
 * 
 * @param :
 * 
 * @return:
 */
int main(){

    int option = 0;
    scanf("%d",&option);
    
    char fileName[MAX_NAME_LENGTH];
    char dirPath[MAX_NAME_LENGTH];

    char fileLogName[MAX_NAME_LENGTH];
    char fileNewName[MAX_NAME_LENGTH];
    int version = 0;


    switch (option){
        case 0://fullBackup
            scanf("%s\n",fileName);
            gets(dirPath);
            printf("path:%s\n",dirPath);
            fullBackup(fileName,dirPath);
            break;

        case 1://backing up the file with the name below for example filename:file.txt -> backupname->file_incremental1.txt and then put it and its inc(log) in a zip file named file_incremental1
            //and if file_incremental1 exists make file_incremental2
            scanf("%s\n",fileName); //fileName to backup ////think about SPACES!!!!!!!!!!
            gets(dirPath);
            backupTheFile(fileName,dirPath);
            break;

        case 2: //restore the version that given blow for example   file'n'.txt -> making the file file_incremental'n'.txt
            scanf("%s\n", fileNewName);
            gets(dirPath);//careful
            scanf("%d", &version);
            printf("calling restore");
            restore(fileNewName, dirPath, version);
            break;

        case 3: //make a backup of all directories ;) maybe next version
            break;
        default:
            break;
    }
    

    return 0;
}


////////////Full Backup//////////////
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

/////////////////////Incremental Backup////////////////////////

typedef struct Array2d{
    char array[LINE_NUMBERS][LINE_LENGTH];
    int  lines;
};

/**
 * this function is calling a " PYTHON " file (i really tried to write it in c but i couldn't) 
 * the python file compare two files and put the result in an inc.txt file
 * then with this function we compress and generate the proper name for the file 
 * 
 * @param filename : its the name of the file that we want to incrementally backup it 
 * @param dirPath  : its the path of where the filename.txt exists.
 * 
 * @return :
 */
void backupTheFile(char *filename, char *dirPath){
    //for example 
    //filename         ->  file.txt 
    //backup name      ->  file1.cab
    //next backup name ->  file2.cab
    //...

    /*
    first we should see if filename'n'.cab exists
    if not we should find filename.cab 
    awsume that filename1.cab 'does not exists' and we have just filename.cap
     then we should extract filename.cpp that contains filename.txt
     then compare filename.txt(that extracted) with the filename.txt(that we want to backup)\
     then make inc1.txt and compress it with the name filename1.cab and keep the last versin --> filename.txt(that we wanted to backup)   
     then make a copy of file filename.txt with the name filename1.txt for next backup

    awsume that filename1.cab 'exists'   
     we previously had filename1.txt
     then we compare filename.txt(we want to backup it) with filename1.txt(keep from last backup)
     and put the result in inc2.txt and cpmpress it to filename2.cap
    
    AND SO ON...
    */

    //so we have to get all directories
    struct Array2d direc = GettinAllDirectories(dirPath);

    char dirName[MAX_NAME_LENGTH];
    char filenameToRE[MAX_NAME_LENGTH];
    int n = 1;
    char copy[MAX_NAME_LENGTH];

    char fileName[MAX_NAME_LENGTH];
    strcpy(fileName, filename);
    strcpy(copy,fileName);
    char *fnwe = getfileNameWithoutExtention(copy);
    char fileNameWithoutExtention[MAX_NAME_LENGTH];
    strcpy(fileNameWithoutExtention, fnwe);

    strcpy(copy,fileName);
    char *ex = getFileExtention(copy);
    char extention[MAX_NAME_LENGTH];
    strcpy(extention, ex);

    
    sprintf(filenameToRE, "%s.cab",fileNameWithoutExtention);//filename.cab

    if(!isFileNameExists(direc, filenameToRE)){
        printf("Make sure you have made full backup first!\n");
        exit(0);
    }

    sprintf(filenameToRE, "%s%d.cab",fileNameWithoutExtention,n);//filename'n'.cab
    while(isFileNameExists(direc, filenameToRE)){
        sprintf(filenameToRE, "%s%d.cab",fileNameWithoutExtention,n);//filename'n'.cab
        n++;
    }

    char command[MAX_NAME_LENGTH * 5];
    if(!isFileNameExists(direc, "temp")){
        sprintf(command, "mkdir \"%s\\temp\"",dirPath);
        system(command);
    }
    
    if(!isFileNameExists(direc, "log")){
        sprintf(command, "mkdir \"%s\\log\"",dirPath);
        system(command);
    }
    
    char log[MAX_NAME_LENGTH * 3];
    sprintf(log,"%s\\log\\log.txt",dirPath);
    FILE *logger = fopen(log, "a");

    if(n-2 == -1){
        //means we just have full backup and we should make 'inc1.txt' and compress it to 'filename1.cab'
        
        //making a file that what we want to give to the python file ;)
        sprintf(command, "echo '%s/%s_n-1%s?%s/%s?%s/temp/inc1.txt' >\"%s\\temp\\temp.txt\"", dirPath, fileNameWithoutExtention, extention, dirPath, fileName, dirPath, dirPath);
        system(command); 

        sprintf(command, "diffWith.py <\"%s\\temp\\temp.txt\"",dirPath);
        system(command); //diffWith.py <temp.txt

        sprintf(command, "makecab \"%s/temp/inc1.txt\" \"%s\\%s1.cab\"", dirPath, dirPath,fileNameWithoutExtention);
        system(command); //makecab inc1.txt filename1.cab

        sprintf(command, "copy \"%s\\%s\" \"%s\\%s_n-1%s\"", dirPath, fileName, dirPath, fileNameWithoutExtention,extention);
        system(command); //cp filename.txt filename_n-1.txt

        sprintf(command, "del \"%s\\temp\\inc1.txt\"", dirPath);
        system(command);

        fputs("INCREMENTAL BACKUP:\t compressed incremental of version'1' created\n", logger);
    }

    else{
        //we should make a backup of file 'filename.txt' from 'filename_n-1.txt' and put it in ('filename'n-2+1'.txt')
        int x = n - 2 + 1;
        sprintf(command, "echo '%s/%s_n-1%s?%s/%s%s",dirPath,fileNameWithoutExtention,extention,dirPath,fileNameWithoutExtention,extention);
        
        char addCommand[MAX_NAME_LENGTH];
        sprintf(addCommand, "?%s/temp/inc%d.txt' >\"%s/temp/temp.txt\"", dirPath, x, dirPath);
        strcat(command, addCommand);
        system(command);//echo '.\file_n-1.txt .\file.txt .\temp\inc2.txt' >.\temp\temp.txt

        sprintf(command, "diffWith.py <\"%s\\temp\\temp.txt\"",dirPath);
        system(command);//diffWith.py <temp.txt

        sprintf(command, "makecab \"%s/temp/inc%d.txt\" \"%s\\%s%d.cab\"", dirPath, x, dirPath,fileNameWithoutExtention, x);
        system(command);//cp bla.txt bla2.txt

        sprintf(command, "del \"%s\\temp\\inc%d.txt\"",dirPath,x);
        system(command); //del inc'n-2+1'.txt

        // sprintf(command, "del \"%s\\%s_n-1%s\"", dirPath, fileNameWithoutExtention,extention);
        // system(command); //del filename_n-1.txt

        sprintf(command, "copy \"%s\\%s\" \"%s\\%s_n-1%s\"", dirPath, fileName, dirPath, fileNameWithoutExtention,extention);
        system(command); //copy filename.txt filename_n-1.txt

        sprintf(log, "INCREMENTAL BACKUP:\t compressed incremental of version'%d' created\n", x);
        fputs(log, logger);
    }

    fclose(logger);
}



/**
 * this function gets all the files and directories that exists in the path .
 * 
 * @param pathToGetSubdirectories : its the path that we want to get its directories .
 * 
 * @return directories: an array of names of all subdirectories . 
 */
struct Array2d GettinAllDirectories(char *pathToGetSubdirectories){

	struct dirent *de;
    struct Array2d directories;
    directories.lines = 0;

    DIR *dr = opendir(pathToGetSubdirectories); 

	if (dr == NULL){ 
		printf("Could not open current directory in GettinAllDirectories\n" ); 
	} 
	
	while ((de = readdir(dr)) != NULL) {

		if ( strcmp(de->d_name, ".")==0 || strcmp(de->d_name, "..")==0 )continue;

        strcpy(directories.array[directories.lines], de->d_name);
        directories.lines++;
    }	
		
	closedir(dr);
    return directories;
} 


/**
 * this function checks if filenameToRE is exists in an array of strings or not .
 * 
 * @param    direc     : a struct that has a 2d array . 
 * @param filenameToRE : the filename that we want to find out if its in 'direc' .
 * 
 * @return : 1 if the filenameToRE is exists in direct(array of strings) and 0 otherwise . 
 */
int isFileNameExists(struct Array2d direc, char filenameToRE[]){
    int counter = 0;
    while (counter<direc.lines){
        if(isStrInStr(direc.array[counter], filenameToRE)){
            return 1;
        }
        counter++;
    }
    return 0;
}


/////////////////////////Restore////////////////////////////////

// /**
//  * this will help us return an array of intigers.
//  */ 
// typedef struct Array2d{
//     char array[LINE_NUMBERS][LINE_LENGTH];
//     int  lines;
// };


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