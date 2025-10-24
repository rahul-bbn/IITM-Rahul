#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#define DB_FILE "data.db"
#define WAL_FILE "wal.log"

char *currentTimestamp()
{
    static char buf[64];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
    return buf;
}

void writeToWal(char *operation, char *key, char *val)
{
    FILE *wal = fopen(WAL_FILE, "a");
    if (wal==NULL)
    {
        perror("failed to open wal!");
        exit(1);
    }
    fprintf(wal, "[%s] %s %s %s\n", currentTimestamp(), operation, key, val ? val : "NULL");
    fclose(wal);
}

void applyToDB(char *operation, char *key, char *val)
{
    FILE *DB = fopen(DB_FILE, "a");
    if (DB == NULL)
    {
        perror("failed to open DB!");
        exit(1);
    }
    fprintf(DB, "%s %s %s\n", operation, key, val ? val : "NULL");
    fclose(DB);
}

void commitOperation(char *operation, char *key, char *val)
{
    writeToWal(operation, key, val);
    applyToDB(operation, key, val);
    printf("committed: %s %s %s\n", operation, key, val ? val : "NULL");
}

void replay()
{
    FILE *repWal = fopen(WAL_FILE, "r");
    if (repWal == NULL)
    {
        printf("There is not found for recovery.\n");
        return;
    }
    printf("\nReplaying wal for recovery...\n");
    char timestamp[64], op[16], key[64], value[128];

    while (fscanf(repWal, "[%[^]]] %s %s %s\n", timestamp, op, key, value) == 4)
    {
        applyToDB(op, key, strcmp(value, "NULL") == 0 ? NULL : value);
        printf("Recovered: %s %s %s\n", op, key, value);
    }
    fclose(repWal);
    printf("Replay completed.\n");
}

int main()
{
    replay();

    commitOperation("INSERT", "user1", "Rahul");
    commitOperation("INSERT", "user2", "Kanha");
    commitOperation("UPDATE", "user1", "Gaurav_update");
    commitOperation("DELETE", "user2", NULL);

    return 0;
}