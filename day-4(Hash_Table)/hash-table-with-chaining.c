#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TABLE_SIZE 10

struct Node {
    char key[50];
    int value;
    struct Node *next;
};

struct Node* hashTable[TABLE_SIZE];

//hash function
unsigned int hash(const char *key)
{
    unsigned int hashValue = 0;

    while (*key)
    {
        hashValue = (hashValue * 31) + *key++;
    }
    return hashValue % TABLE_SIZE;
}

//insert
void insert(const char *key, int value)
{
    unsigned int index = hash(key);

    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
    strcpy(newNode->key, key);
    newNode->value = value;
    newNode->next = NULL;

    if(hashTable[index] == NULL)
    {
        hashTable[index] = newNode;
    }
    else
    {
        struct Node *temp = hashTable[index];
        while(temp->next != NULL)
        {
            if(strcmp(temp->key, key) == 0)
            {
                temp->value = value;
                free(newNode);
            }
            temp = temp->next;
        }
    
        if(strcmp(temp->key, key) == 0)
        {
            temp->value = value;
            free(newNode);
            return;
        }
        temp->next = newNode;
    }  
    printf("(%s, %d) is successfully inserted at the index %u \n", key, value, index); 
}

//search
int search(const char *key)
{
    unsigned int index = hash(key);
    struct Node *temp = hashTable[index];

    while (temp != NULL)
    {
        if(strcmp(temp->key, key) == 0)
        {
            return temp->value;
        }
        temp = temp->next;
    }
    return -1;
}

//delete
void delete(const char *key)
{
    unsigned int index = hash(key);
    struct Node *temp = hashTable[index];
    struct Node *prev = NULL;

    while (temp != NULL)
    {
        if(strcmp(temp->key, key) == 0)
        {
            if(prev == NULL)
            {
                hashTable[index] = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }
            free(temp);
            printf("Deleted the data with the key %s \n", key);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("The key %s is not found \n", key);
}

//display
void display()
{
    for (int iterator = 0; iterator < TABLE_SIZE; iterator++) {
        printf("Index %d: ", iterator);
        struct Node *temp = hashTable[iterator];
        while (temp != NULL) {
            printf("(%s, %d) -> ", temp->key, temp->value);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

int main() {
    insert("apple", 100);
    display();

    insert("banana", 200);
    display();

    insert("orange", 300);
    display();

    insert("grapes", 400);
    display();

    insert("melon", 500);
    display();
    

    printf("Search 'banana': %d\n", search("banana"));
    printf("Search 'berry': %d\n", search("berry"));

    delete("orange");
    display();

    return 0;
}