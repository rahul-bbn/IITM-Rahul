#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TABLE_SIZE 10
#define EMPTY_KEY NULL
#define DELETED_KEY ((char*) - 1)

struct Entry {
    char *key;
    int value;
};

struct Entry hashTable[TABLE_SIZE];

//hash
unsigned int hash(const char *key)
{
    unsigned int hashValue = 0;

    while (*key)
    {
        hashValue = (hashValue * 31) + *key++;
    }
    return hashValue % TABLE_SIZE;
}

//init
void init() {
    for (int iterator = 0; iterator < TABLE_SIZE; iterator++)
    {
        hashTable[iterator].key = EMPTY_KEY;
        hashTable[iterator].value = 0;
    }
}

//insert
void insert(const char *key, int value)
{
    unsigned int index = hash(key);
    unsigned int originalIndex = index;

    while (hashTable[index].key != EMPTY_KEY &&
            hashTable[index].key != DELETED_KEY && 
            strcmp(hashTable[index].key, key) != 0)
    {
        index = (index + 1) % TABLE_SIZE;
        if( index == originalIndex)
        {
            printf("%s can't be inserted as the table is full \n", key);
            return;
        }
    }
        
    if(hashTable[index].key != EMPTY_KEY && hashTable[index].key != DELETED_KEY)
    {
        hashTable[index].value = value;
    }
    else
    {
        hashTable[index].key = strdup(key);
        hashTable[index].value = value;
    }

    printf("%s : %d is inserted successfully at the index %u \n", key, value, index);
}

//search
int search(const char *key)
{
    unsigned int index = hash(key);
    unsigned int originalIndex = index;

    while (hashTable[index].key != EMPTY_KEY)
    {
        if(hashTable[index].key != DELETED_KEY && strcmp(hashTable[index].key, key) == 0)
        {
            return hashTable[index].value;
        }

        index = ( index + 1 ) % TABLE_SIZE;

        if(index == originalIndex) break;
    }

    return -1;         
}

//delete

void delete(const char *key)
{
    unsigned int index = hash(key);
    unsigned int originalIndex = index;

    while (hashTable[index].key != EMPTY_KEY)
    {
        if(hashTable[index].key != DELETED_KEY && strcmp(hashTable[index].key, key) == 0)
        {
            free(hashTable[index].key);
            hashTable[index].key = DELETED_KEY;
            printf("The key %s at the index %u is deleted \n", key, index);
            return;
        }

        index = (index + 1) % TABLE_SIZE;

        if(index == originalIndex) break;
    }

    printf("The key %s is not found \n", key);
}

//display
void display()
{
    for (int iterator = 0; iterator < TABLE_SIZE; iterator++)
    {
        if(hashTable[iterator].key == EMPTY_KEY)
        {
            printf("Index %d : EMPTY \n", iterator);
        }
        else if(hashTable[iterator].key == DELETED_KEY)
        {
            printf("Index %d : DELETED \n", iterator);
        }
        else
        {
            printf("Index %d : (%s, %d) \n", iterator, hashTable[iterator].key, hashTable[iterator].value);
        }
    }
}

//main

int main()
{
    init();
    display();

    insert("apple", 100);
    display();

    insert("banana", 200);
    display();

    insert("pineapple", 300);
    display();

    insert("watermelon", 400);
    display();

    insert("orange", 500);
    display();

    printf("Search 'Banana' : %d \n", search("banana"));
    printf("Search 'Watermelon' : %d \n", search("watermelon"));

    delete("orange");
    display();

    insert("strawberry", 600);
    display();
    
    return 0;
}