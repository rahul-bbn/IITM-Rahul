#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>

int main(){
    char dirName[256];
    printf("Enter directory name to scan: ");
    scanf("%s",dirName);

    DIR *dir=opendir(dirName);
    if (dirName==NULL)
    {
        perror("could not open directory!");
        exit(1);
    }

    struct dirent *entry;

    int txtCount=0;
    int cCount=0;
    int logCount=0;
    int otherCount=0;

    while ((entry=readdir(dir))!=NULL)
    {
        if (entry->d_type == DT_DIR) continue; //// Skip directories "." and ".."

        char *dot=strrchr(entry->d_name,'.');
        if (dot==NULL)
        {
            otherCount++;
        }else if (strcmp(dot,".txt")==0)
        {
            txtCount++;
        }else if (strcmp(dot,".log")==0)
        {
            logCount++;
        }else if (strcmp(dot,".c"))
        {
            cCount++;
        }else{
            otherCount++;
        }
    }
    closedir(dir);

    printf("\nFile counts in directory '%s':\n", dirName);
    printf(".txt files: %d\n", txtCount);
    printf(".c files: %d\n", cCount);
    printf(".log files: %d\n", logCount);
    printf("Other files: %d\n", otherCount);

    return 0;
}