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

//inorder - Left->Root->Right
void inorder(struct Node* root)
{
    if(root == NULL)
    {
        return ;
    }
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

//preorder - Root->Left->Right
void preorder(struct Node* root)
{
    if(root == NULL)
    {
        return ;
    }

    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

//postorder - Left->Right->Root
void postorder(struct Node* root)
{
    if(root == NULL)
    {
        return ;
    }

    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

int main()
{
    struct Node* root = createNode(1);
    
    root->left = createNode(2);
    root->right = createNode(3);

    root->left->left = createNode(4);
    root->left->right = createNode(5);

    root->right->left = createNode(6);
    root->right->right = createNode(7);

    printf("Inorder Traversal : \n");
    inorder(root);
    printf("\n ------------ \n");

    printf("Preorder Traversal : \n");
    preorder(root);
    printf("\n ------------ \n");

    printf("Postorder Traversal : \n");
    postorder(root);
    printf("\n ------------ \n");

    return 0;
}

