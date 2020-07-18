#include "incremental.h"



#define MAX_NAME_LENGTH 100
#define LINE_NUMBERS    200
#define LINE_LENGTH     MAX_NAME_LENGTH


typedef struct Array2d{
    char array[LINE_NUMBERS][LINE_LENGTH];
    int  lines;
};

// typedef struct Array{ 
//     char *array;
//     int  lines;
// };

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


