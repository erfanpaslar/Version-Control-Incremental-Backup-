#include "fullBackup.h"
#include "restore.h"
#include "incremental.h"


#define MAX_NAME_LENGTH 100
#define LINE_NUMBERS    200
#define LINE_LENGTH     MAX_NAME_LENGTH

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


