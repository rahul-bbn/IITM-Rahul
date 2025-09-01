// #include<stdio.h>
// #include<stdlib.h>
// #include<time.h>
// #define MAX_LEVEL 6
// #define P_FACTOR 0.5
// int i;

// typedef struct skipListNode{
//     int key;
//     struct skipListNode **forward;
// }skipListNode;

// typedef struct skipList{
//     int level;
//     skipListNode* header;
// }skipList;

// //create Node
// skipListNode* createNode(int key,int level){
//     skipListNode* node=(skipListNode*)malloc(sizeof(skipListNode));
//     node->key=key;
//     node->forward=(skipListNode**)malloc(sizeof(skipListNode*)*(level+1));

//     for (int  i = 0; i < level; i++)
//     {
//         node->forward[i]=NULL;
//     }
//     return node;
// }

// //create list
// skipList* createSkipList(){
//     skipList* list=(skipList*)malloc(sizeof(skipList));
//     list->header=createNode(-1,MAX_LEVEL);

//     return list;
// }

// //generate Random level
// int geneateRandomLevel(){
//     int level=0;
//     srand(time(NULL));
//     while ((rand()/(double)RAND_MAX) < P_FACTOR && level<MAX_LEVEL)
//     {
//         level++;
//     }
//     return level;
// }

// //search
// skipListNode* search(skipList* list,int key){
//     skipListNode* current=list->header;

//     for (int  i = list->level; i >= 0; i--)
//     {
//        while (current->forward[i] && current->forward[i]->key < key)
//        {
//         current=current->forward[i];
//        }
//     }
//     current=current->forward[i];
//     if(current && current->key == key){
//         return current;
//     }
//     return NULL;
// }

// //insert
// void insert(skipList* list,int key){
//     skipListNode* update[MAX_LEVEL-1];
//     skipListNode* current =list->header;

//     for (int i = list->level; i >= 0; i--)
//     {
//         while (current->forward[i] && current->forward[i]->key > key)
//         {
//             current=current->forward[i];
//         }
//         update[i]=current;
//     }
//     current=current->forward[0];

//     if(!current || current->key!=key){
//         int newLevel= geneateRandomLevel();
//         if (newLevel > list->level)
//         {
//             for (int i = list->level+1; i >=newLevel; i++)
//             {
//                 update[i]=list->header;
//             }
//             list->level=newLevel;
//         }
//         skipListNode *newNode=createNode(key,newLevel);

//         for (int i = 0; i <= newLevel; i++)
//         {
//             newNode->forward[i]=update[i]->forward[i];
//             update[i]->forward[i]=newNode;
//         }   
//     }
// }

// //display
// void display(skipList* list){
//     printf("--------------Skip List-------------");
//     for (int i = list->level; i >=0; i--)
//     {
//         skipList* node=list->header->forward[i];
//         printf("level %d: ",i);
//         while (node)
//         {
//             printf("%d ", node->key);
//             node = node->forward[i];
//         }
//         printf("\n");
//     }
    
// }

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LEVEL 6
#define P_FACTOR 0.5

typedef struct skipListNode {
    int key;
    struct skipListNode **forward;
} skipListNode;

typedef struct skipList {
    int level;
    skipListNode* header;
} skipList;

// create Node
skipListNode* createNode(int key, int level) {
    skipListNode* node = (skipListNode*)malloc(sizeof(skipListNode));
    node->key = key;
    node->forward = (skipListNode**)malloc(sizeof(skipListNode*) * (level + 1));
    for (int i = 0; i <= level; i++) {
        node->forward[i] = NULL;
    }
    return node;
}

// create list
skipList* createSkipList() {
    skipList* list = (skipList*)malloc(sizeof(skipList));
    list->level = 0;
    list->header = createNode(-1, MAX_LEVEL); // dummy key -1
    return list;
}

// generate Random level
int generateRandomLevel() {
    int level = 0;
    while (((double)rand() / RAND_MAX) < P_FACTOR && level < MAX_LEVEL) {
        level++;
    }
    return level;
}

// search
skipListNode* search(skipList* list, int key) {
    skipListNode* current = list->header;
    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    if (current && current->key == key) {
        return current;
    }
    return NULL;
}

// insert
void insert(skipList* list, int key) {
    skipListNode* update[MAX_LEVEL + 1];
    skipListNode* current = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];

    if (!current || current->key != key) {
        int newLevel = generateRandomLevel();
        if (newLevel > list->level) {
            for (int i = list->level + 1; i <= newLevel; i++) {
                update[i] = list->header;
            }
            list->level = newLevel;
        }
        skipListNode *newNode = createNode(key, newLevel);
        for (int i = 0; i <= newLevel; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }
}

//Delete node
void delete(skipList* list, int key){
    skipListNode* update[MAX_LEVEL+1];
    skipListNode* current=list->header;

    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    // Step 2: Move to possible target node
    current = current->forward[0];

    // Step 3: If found, adjust pointers and delete
    if (current && current->key==key)
    {
        for (int  i = 0; i <= list->level; i++)
        {
            if (current!=update[i]->forward[i])
            {
                break;  // stop once this level no longer points to the node
            }
                update[i]->forward[i]=current->forward[i];
        }
        free(current);

        // Step 4: Reduce list level if top levels are empty
        while (list->level > 0 && list->header->forward[list->level] == NULL) {
            list->level--;
        }
        
    } 
}

// display
void display(skipList* list) {
    printf("\n------------- Skip List -------------\n");
    for (int i = list->level; i >= 0; i--) {
        skipListNode* node = list->header->forward[i];
        printf("Level %d: ", i);
        while (node) {
            printf("%d ", node->key);
            node = node->forward[i];
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));

    skipList* list = createSkipList();

    insert(list, 3);
    insert(list, 6);
    insert(list, 7);
    insert(list, 9);
    insert(list, 12);
    insert(list, 19);
    insert(list, 17);
    insert(list, 26);
    insert(list, 21);
    insert(list, 25);

    display(list);

    int key = 19;
    skipListNode* found = search(list, key);
    if (found) {
        printf("Found key %d in skip list.\n", key);
    } else {
        printf("Key %d not found.\n", key);
    }
    
    delete(list,17);
    display(list);

    return 0;
}
