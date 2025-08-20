#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define MAX 3

typedef struct BTreeNode{
    int keys[MAX];
    struct BTreeNode* child[MAX+1];
    int count;
    bool isleaf;
}BTreeNode;

BTreeNode *createNode(bool isleaf){
    BTreeNode *newNode=(BTreeNode*)malloc(sizeof(BTreeNode));
    newNode->isleaf=isleaf;
    newNode->count=0;
    for (int i = 0; i < MAX; i++)
    {
        newNode->child[i]=NULL;
    }
    return newNode; 
}

bool searchKey(int keyToBeSearched,BTreeNode *root){
    if (root==NULL)
    {
        return false;
    }
    int i=0;

    while (i<root->count && keyToBeSearched > root->keys[i])
    {
        i++;
    }
    if(i<root->count && keyToBeSearched == root->keys[i]){
        return true;
    }
    if (root->isleaf)
    {
        return false;
    }
    return searchKey(keyToBeSearched,root->child[i]);
}

int main(){
    BTreeNode* root=createNode(false);

    root->keys[0]=10;
    root->keys[1]=20;
    root->count=2;

    root->child[0]=createNode(true);
    root->child[1]=createNode(true);
    root->child[2]=createNode(true);

    root->child[0]->keys[0] = 5;
    root->child[0]->keys[1] = 8;
    root->child[0]->count = 2;

    root->child[1]->keys[0] = 12;
    root->child[1]->keys[1] = 15;
    root->child[1]->keys[2] = 18;
    root->child[1]->count = 3;

    root->child[2]->keys[0] = 25;
    root->child[2]->keys[1] = 30;
    root->child[2]->count = 2;

    int keyToSearch[] = {15, 9, 25, 100};

    for (int iterator = 0; iterator < 4; iterator++)
    {
        int keyToBeSearched = keyToSearch[iterator];

        if(searchKey(keyToBeSearched, root))
        {
            printf("%d key is found in the B-Tree \n", keyToBeSearched);
        }
        else
        {
            printf("%d key is not found in the B-Tree \n", keyToBeSearched);
        }
    }
    return 0;
}


// Root : [10,20]
//  |- Child [0] : [5,8]
//  |- Child [1] : [12, 15, 18]
//  |_ Child [2] : [25, 30]