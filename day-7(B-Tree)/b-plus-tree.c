#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX 3   // max keys per node

typedef struct BPlusTreeNode {
    int keys[MAX];
    struct BPlusTreeNode* child[MAX+1];
    int count;
    bool isleaf;
    struct BPlusTreeNode* next;
} BPlusTreeNode;

// Create node
BPlusTreeNode* createNode(bool isLeaf) {
    BPlusTreeNode* node = malloc(sizeof(BPlusTreeNode));
    node->isleaf = isLeaf;
    node->count = 0;
    node->next = NULL;
    for (int i = 0; i < MAX+1; i++) node->child[i] = NULL;
    return node;
}

// Insert into a leaf
void insertIntoLeaf(BPlusTreeNode* leaf, int key) {
    int i = leaf->count - 1;
    while (i >= 0 && key < leaf->keys[i]) {
        leaf->keys[i+1] = leaf->keys[i];
        i--;
    }
    leaf->keys[i+1] = key;
    leaf->count++;
}

// Split leaf
BPlusTreeNode* splitLeaf(BPlusTreeNode* leaf, int* newKey) {
    int mid = (MAX+1)/2;
    BPlusTreeNode* newLeaf = createNode(true);

    for (int i = mid, j = 0; i < MAX; i++, j++) {
        newLeaf->keys[j] = leaf->keys[i];
        newLeaf->count++;
    }
    leaf->count = mid;

    newLeaf->next = leaf->next;
    leaf->next = newLeaf;

    *newKey = newLeaf->keys[0];
    return newLeaf;
}

// Insert into internal node
void insertIntoInternal(BPlusTreeNode* internal, int key, BPlusTreeNode* child) {
    int i = internal->count - 1;
    while (i >= 0 && key < internal->keys[i]) {
        internal->keys[i+1] = internal->keys[i];
        internal->child[i+2] = internal->child[i+1];
        i--;
    }
    internal->keys[i+1] = key;
    internal->child[i+2] = child;
    internal->count++;
}

// Split internal node
BPlusTreeNode* splitInternal(BPlusTreeNode* internal, int* newKey) {
    int mid = internal->count / 2;
    BPlusTreeNode* newInternal = createNode(false);

    for (int i = mid+1, j = 0; i < internal->count; i++, j++) {
        newInternal->keys[j] = internal->keys[i];
        newInternal->child[j] = internal->child[i];
        newInternal->count++;
    }
    newInternal->child[newInternal->count] = internal->child[internal->count];

    *newKey = internal->keys[mid];
    internal->count = mid;

    return newInternal;
}

// Recursive insert
BPlusTreeNode* insert(BPlusTreeNode* root, int key, int* promoted, bool* grew) {
    if (root->isleaf) {
        if (root->count < MAX) {
            insertIntoLeaf(root, key);
            *grew = false;
            return NULL;
        } else {
            insertIntoLeaf(root, key);
            BPlusTreeNode* newLeaf = splitLeaf(root, promoted);
            *grew = true;
            return newLeaf;
        }
    } else {
        int i = 0;
        while (i < root->count && key > root->keys[i]) i++;

        int newKey;
        bool childGrew;
        BPlusTreeNode* newChild = insert(root->child[i], key, &newKey, &childGrew);

        if (childGrew) {
            if (root->count < MAX) {
                insertIntoInternal(root, newKey, newChild);
                *grew = false;
                return NULL;
            } else {
                insertIntoInternal(root, newKey, newChild);
                BPlusTreeNode* newInternal = splitInternal(root, promoted);
                *grew = true;
                return newInternal;
            }
        }
        *grew = false;
        return NULL;
    }
}

// Wrapper for insertion
BPlusTreeNode* insertKey(BPlusTreeNode* root, int key) {
    if (!root) {
        root = createNode(true);
        insertIntoLeaf(root, key);
        return root;
    }
    int promoted;
    bool grew;
    BPlusTreeNode* newChild = insert(root, key, &promoted, &grew);
    if (grew) {
        BPlusTreeNode* newRoot = createNode(false);
        newRoot->keys[0] = promoted;
        newRoot->child[0] = root;
        newRoot->child[1] = newChild;
        newRoot->count = 1;
        return newRoot;
    }
    return root;
}

// Search
bool search(BPlusTreeNode* root, int key) {
    int i = 0;
    while (i < root->count && key > root->keys[i]) i++;

    if (root->isleaf) {
        for (int j = 0; j < root->count; j++)
            if (root->keys[j] == key) return true;
        return false;
    }
    return search(root->child[i], key);
}

// Print all leaves
void printLeaves(BPlusTreeNode* root) {
    while (!root->isleaf) root = root->child[0];
    while (root) {
        for (int i = 0; i < root->count; i++)
            printf("%d ", root->keys[i]);
        root = root->next;
    }
    printf("\n");
}

// Print keys in range [start, end]
void printRange(BPlusTreeNode* root, int start, int end) {
    while (!root->isleaf) {
        int i = 0;
        while (i < root->count && start > root->keys[i]) i++;
        root = root->child[i];
    }
    while (root) {
        for (int i = 0; i < root->count; i++) {
            if (root->keys[i] >= start && root->keys[i] <= end)
                printf("%d ", root->keys[i]);
            if (root->keys[i] > end) return;
        }
        root = root->next;
    }
}

int main() {
    BPlusTreeNode* root = NULL;

    // Insert keys
    int arr[] = {10, 20, 5, 6, 12, 30, 7, 17, 25, 40};
    int n = sizeof(arr)/sizeof(arr[0]);
    for (int i = 0; i < n; i++) {
        root = insertKey(root, arr[i]);
    }

    printf("All leaf keys in order: ");
    printLeaves(root);

    // Search tests
    int keysToSearch[] = {6, 15, 25};
    for (int i = 0; i < 3; i++) {
        if (search(root, keysToSearch[i]))
            printf("%d found\n", keysToSearch[i]);
        else
            printf("%d not found\n", keysToSearch[i]);
    }

    // Print range
    printf("Keys in range [10, 25]: ");
    printRange(root, 10, 25);
    printf("\n");

    return 0;
}