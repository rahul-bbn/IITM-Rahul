#include<stdio.h>
#include<stdlib.h>

struct Node
{
    int data;
    struct Node* left;
    struct Node* right;
};

//createNode
struct Node* createNode(int value)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));

    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

// insert level order
struct Node* insertLevelOrder(int array[], int index, int n)
{
    if(index >= n)
    {
        return NULL;
    }

    struct Node* root = createNode(array[index]);
    root->left = insertLevelOrder(array, 2 * index + 1, n);
    root->right = insertLevelOrder(array, 2 * index + 2, n);

    return root;
}

//findDepth
int findDepth(struct Node* root)
{
    if(root == NULL)
    {
        return 0;
    }

    int leftDepth = findDepth(root->left);
    int rightDepth = findDepth(root->right);

    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

int main()
{
    int array[] = {1,2,3,4,5,6,7};
    int n = sizeof(array)/sizeof(array[0]);

    struct Node* root = insertLevelOrder(array, 0, n);

    int depth = findDepth(root);
    printf("Depth of the tree is : %d \n", depth);
    
    return 0;
}