// Write a C program that creates, writes, and reads multiple
// files in a directory. Ensure proper error handling (check if files
// exist before writing).

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define DIR_NAME "my_files"
#define NUM_FILES 3

int main() {
    if (mkdir(DIR_NAME, 0777) <0) {
        if (errno != EEXIST) {
            perror("Error creating directory");
            exit(1);
        }
    }

    char filePath[256];
    FILE *fp;

    for (int i = 1; i <= NUM_FILES; i++) {
        sprintf(filePath, "%s/file%d.txt", DIR_NAME, i);

        fp = fopen(filePath, "r");
        if (fp != NULL) {
            printf("File %s already exists, skipping write.\n", filePath);
            fclose(fp);
            continue;
        }

        fp = fopen(filePath, "w");
        if (fp == NULL) {
            perror("Error opening file for writing");
            continue;
        }

        fprintf(fp, "This is file number %d\n", i);
        fclose(fp);
        printf("Written to %s successfully.\n", filePath);
    }

    printf("\n--- Reading files ---\n");
    char buffer[256];
    for (int i = 1; i <= NUM_FILES; i++) {
        sprintf(filePath, "%s/file%d.txt", DIR_NAME, i);

        fp = fopen(filePath, "r");
        if (fp == NULL) {
            perror("Error opening file for reading");
            continue;
        }

        printf("Contents of %s:\n", filePath);
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            printf("%s", buffer);
        }
        printf("\n");

        fclose(fp);
    }

    return 0;
}
