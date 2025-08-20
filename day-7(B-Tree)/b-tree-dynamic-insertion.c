#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define MAX 3

typedef struct BTreeNode {
   int keys[MAX];
   int count;
   struct BTreeNode* child[MAX+1];
   bool isLeaf;
} BTreeNode;

//createNode
BTreeNode* createNode(bool isLeaf) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->isLeaf = isLeaf;
    node->count = 0;
    for (int i = 0; i <= MAX; i++) {
        node->child[i] = NULL;
    }
    return node;
}

//search
bool searchKey(int key, BTreeNode* root) {
    if(root == NULL) return false;
    int i = 0;

    while (i < root->count && key > root->keys[i]) {
        i++;
    }
    if (i < root->count && key == root->keys[i]) return true;
    if (root->isLeaf) return false;

    return searchKey(key, root->child[i]);
}

//split()
void splitChild(BTreeNode* parent, int idx, BTreeNode* fullChild) {
    int mid = MAX/2;
    BTreeNode* newNode = createNode(fullChild->isLeaf);
    newNode->count = MAX - mid - 1;

    for (int j = 0; j < newNode->count; j++) {
        newNode->keys[j] = fullChild->keys[mid + 1 + j];
    }
    if (!fullChild->isLeaf) {
        for (int j = 0; j <= newNode->count; j++) {
            newNode->child[j] = fullChild->child[mid + 1 + j];
        }
    }

    fullChild->count = mid;

    for (int j = parent->count; j >= idx+1; j--) {
        parent->child[j+1] = parent->child[j];
    }
    parent->child[idx+1] = newNode;

    for (int j = parent->count-1; j >= idx; j--) {
        parent->keys[j+1] = parent->keys[j];
    }

    parent->keys[idx] = fullChild->keys[mid];
    parent->count++;
}

void insertWhenNonFull(BTreeNode* node, int key) {
    int i = node->count - 1;
    if (node->isLeaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i+1] = node->keys[i];
            i--;
        }
        node->keys[i+1] = key;
        node->count++;
    } else {
        while (i >= 0 && key < node->keys[i]) i--;
        i++;
        if (node->child[i]->count == MAX) {
            splitChild(node, i, node->child[i]);
            if (key > node->keys[i]) i++;
        }
        insertWhenNonFull(node->child[i], key);
    }
}

//insert()
BTreeNode* insert(int key, BTreeNode* root) {
    if (root == NULL) {
        root = createNode(true);
        root->keys[0] = key;
        root->count = 1;
        return root;
    }
    if (root->count == MAX) {
        BTreeNode* newRoot = createNode(false);
        newRoot->child[0] = root;
        splitChild(newRoot, 0, root);

        int i = 0;
        if (key > newRoot->keys[0]) i++;
        insertWhenNonFull(newRoot->child[i], key);
        return newRoot;
    } else {
        insertWhenNonFull(root, key);
        return root;
    }
}

//display()
void display(BTreeNode* root, int level) {
    if (root == NULL) return;
    for (int i = 0; i < level; i++) printf("    ");

    printf("[");
    for (int i = 0; i < root->count; i++) {
        printf("%d", root->keys[i]);
        if (i < root->count - 1) printf(" | ");
    }
    printf("]\n");

    for (int i = 0; i <= root->count; i++) {
        if (root->child[i] != NULL) {
            display(root->child[i], level+1);
        }
    }
}

int main() {
    BTreeNode* root = NULL;
    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int size = sizeof(keys)/sizeof(keys[0]);

    for (int i = 0; i < size; i++) {
        root = insert(keys[i], root);
    }

    printf("Data inserted successfully \n");
    display(root, 0);

    int keysToSearch[] = {6, 15, 30};
    int searchSize = sizeof(keysToSearch)/sizeof(keysToSearch[0]);

    for (int i = 0; i < searchSize; i++) {
        if (searchKey(keysToSearch[i], root)) {
            printf("%d key is found in the B-Tree \n", keysToSearch[i]);
        } else {
            printf("%d key is not found in the B-Tree \n", keysToSearch[i]);
        }
    }
    return 0;
}