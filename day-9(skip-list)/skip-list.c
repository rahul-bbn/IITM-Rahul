#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX_LEVEL 6
#define P_FACTOR 0.5

typedef struct SkipListNode {
    int key;
    struct SkipListNode **forward;
}SkipListNode;

typedef struct SkipList {
    int level;
    SkipListNode *header;
}SkipList;

//createNode
SkipListNode* createNode(int key, int level)
{
    SkipListNode *node = (SkipListNode*)malloc(sizeof(SkipListNode));
    node->key = key;

    node->forward = (SkipListNode**)malloc(sizeof(SkipListNode*) * (level + 1));

    for (int iterator = 0; iterator <= level; iterator++)
    {
        node->forward[iterator] = NULL;
    }

    return node;
}

//createSkipList
SkipList* createSkipList()
{
    SkipList *list = (SkipList*)malloc(sizeof(SkipList));

    list->level=0;
    list->header = createNode(-1, MAX_LEVEL);

    return list;
}

//random
int generateRandomLevel()
{
    int level = 0;

    while((rand()/(double)RAND_MAX) < P_FACTOR && level < MAX_LEVEL)
    {
        level++;
    }

    return level;
}

//search
SkipListNode* search(SkipList *list, int key)
{
    SkipListNode *current = list->header;

    for(int iterator = list->level; iterator>=0; iterator--)
    {
        while(current->forward[iterator] && current->forward[iterator]->key < key)
        {
            current = current->forward[iterator];
        }
    }

    current = current->forward[0];

    if(current && current->key == key)
    {
        return current;
    }

    return NULL;
}

//insert
void insert(SkipList* list, int key)
{
    SkipListNode *update[MAX_LEVEL+1];
    SkipListNode *current = list->header;

    for (int iterator = list->level; iterator >= 0; iterator--)
    {
        while(current->forward[iterator] && current->forward[iterator]->key < key)
        {
            current = current->forward[iterator];
        }
        update[iterator] = current;
    }

    current = current->forward[0];

    if(!current || current->key != key)
    {
        int newLevel = generateRandomLevel();

        if(newLevel > list->level)
        {
            for(int iterator = list->level; iterator <= newLevel; iterator++)
            {
                update[iterator] = list->header;
            }
            list->level = newLevel;
        }

        SkipListNode *newNode = createNode(key, newLevel);

        for(int iterator = 0; iterator <= newLevel; iterator++)
        {
            newNode->forward[iterator] = update[iterator]->forward[iterator];
            update[iterator]->forward[iterator] = newNode;
        }
    }
}

//delete
void delete(SkipList *list, int key)
{
    SkipListNode *update[MAX_LEVEL + 1];
    SkipListNode *current = list->header;

    for (int iterator = list->level; iterator >= 0; iterator--)
    {
        while(current->forward[iterator] && current->forward[iterator]->key < key)
        {
            current = current->forward[iterator];
        }
        update[iterator] = current;
    }

    current = current->forward[0];

    if (current && current->key == key)
    {
       for(int iterator = 0; iterator <= list->level; iterator++)
       {
        if(current != update[iterator]->forward[iterator]) break;
        update[iterator]->forward[iterator] = current->forward[iterator];
       }

       free(current->forward);
       free(current);

       while(list->level > 0 && list->header->forward[list->level] == NULL)
       {
        list->level--;
       }
    }  
}

//display
void display(SkipList *list)
{
    printf("----------SKIP LIST-----------\n");

    for (int iterator = list->level; iterator >= 0; iterator--)
    {
        SkipListNode *node = list->header->forward[iterator];
        printf("Level %d : \n", iterator);

        while (node)
        {
            printf("%d ", node->key);
            node = node->forward[iterator];
        }
        printf("\n");
    }
}

int main()
{

    srand(time(NULL));

    SkipList *list = createSkipList();

    insert(list, 10);
    insert(list, 20);
    insert(list, 30);
    insert(list, 40);
    insert(list, 25);

    display(list);

    printf("\n Searching for 25 : %s \n", search(list, 25) ? "Found" : "Not Found");

    printf("\n Searching for 28 : %s \n", search(list, 28) ? "Found" : "Not Found");

    printf("\n Deleting for 25..\n");

    delete(list, 25);

    display(list);

    printf("\n Searching for 25 : %s \n", search(list, 25) ? "Found" : "Not Found");

    return 0;
}