/*Write a C program that reads the WAL file and replays all
operations to reconstruct the database in-memory. Print a
summary of the final key-value store.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#define DB_FILE "data.db"
#define WAL_FILE "wal1.log"

#define MAX_KEYS 100

typedef struct
{
    char key[64];
    char value[128];
    int valid; // 1 = exists, 0 = deleted
} Record;

Record db[MAX_KEYS];
int db_size = 0;

char *currentTimestamp()
{
    static char buf[64];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
    return buf;
}

void writeToWal(const char *operation, const char *key, const char *val)
{
    FILE *wal = fopen(WAL_FILE, "a");
    if (!wal)
    {
        perror("failed to open wal!");
        exit(1);
    }
    fprintf(wal, "[%s] %s %s %s\n", currentTimestamp(), operation, key, val ? val : "NULL");
    fclose(wal);
}

void applyOperation(const char *operation, const char *key, const char *val)
{
    int found = -1;
    for (int i = 0; i < db_size; i++)
    {
        if (strcmp(db[i].key, key) == 0)
        {
            found = i;
            break;
        }
    }

    if (strcmp(operation, "INSERT") == 0)
    {
        if (found == -1 && db_size < MAX_KEYS)
        {
            strcpy(db[db_size].key, key);
            strcpy(db[db_size].value, val);
            db[db_size].valid = 1;
            db_size++;
        }
        else if (found != -1)
        {
            strcpy(db[found].value, val);
            db[found].valid = 1;
        }
    }
    else if (strcmp(operation, "UPDATE") == 0 && found != -1)
    {
        strcpy(db[found].value, val);
        db[found].valid = 1;
    }
    else if (strcmp(operation, "DELETE") == 0 && found != -1)
    {
        db[found].valid = 0;
    }
}

void commitOperation(const char *operation, const char *key, const char *val)
{
    writeToWal(operation, key, val);
    applyOperation(operation, key, val);
    printf("Committed: %s %s %s\n", operation, key, val ? val : "NULL");
}

void replay()
{
    FILE *repWal = fopen(WAL_FILE, "r");
    if (repWal == NULL)
    {
        printf("No WAL file found for recovery.\n");
        return;
    }

    printf("\nReplaying WAL for recovery...\n");
    char timestamp[64], op[16], key[64], value[128];

    while (fscanf(repWal, "[%[^]]] %s %s %s\n", timestamp, op, key, value) == 4)
    {
        applyOperation(op, key, strcmp(value, "NULL") == 0 ? "" : value);
        printf("Recovered: %s %s %s\n", op, key, value);
    }
    fclose(repWal);
    printf("Replay completed.\n\n");
}

void printDBSummary()
{
    printf("---------- Final In-Memory Key-Value Store -------\n");
    for (int i = 0; i < db_size; i++)
    {
        if (db[i].valid)
            printf("%s => %s\n", db[i].key, db[i].value);
    }
}

int main()
{
    replay();

    commitOperation("INSERT", "user1", "Rahul");
    commitOperation("INSERT", "user2", "Kanha");
    commitOperation("UPDATE", "user1", "Gaurav_update");
    commitOperation("DELETE", "user2", NULL);
    commitOperation("INSERT", "user3", "Sita");

    printf("\n");
    printDBSummary();

    return 0;
}
