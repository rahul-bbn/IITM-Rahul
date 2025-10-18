// Write a C program that reads integers from user input, inserts
// them into a BST, and prints the depth of each node along with
// in-order traversal.

#include<stdio.h>
#include<stdlib.h>

struct Node{
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* createNode(int val){
    struct Node* newNode=(struct Node*)malloc(sizeof(struct Node));
    newNode->data=val;
    newNode->left=NULL;
    newNode->right=NULL;

    return newNode;
}

struct Node* insert(struct Node* root,int val){
    if (root==NULL)
    {
        return createNode(val);
    }
    if (val<root->data)
    {
        root->left=insert(root->left,val);
    }else if (val>root->data)
    {
       root->right=insert(root->right,val);
    }
    return root;
}

void inOrder_depth(struct Node* root,int depth){
    if (root==NULL)
    {
        return;
    }
    inOrder_depth(root->left,depth+1);
    printf("Node: %d, Depth: %d\n",root->data,depth);
    inOrder_depth(root->right,depth+1);
    
}

int main(){
    struct Node* root=NULL;
    int num,val;

    printf("Enter number of elements to insert into bst:");
    scanf("%d",&num);
    printf("Enter %d elements into bst\n",num);
    for (int i = 0; i < num; i++)
    {
        scanf("%d",&val);
        root=insert(root,val);
    }

    printf("\nInOrder traversal\n");
    inOrder_depth(root,0); //root is at depth 0

    return 0;
    
}
