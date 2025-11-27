#include<stdio.h>
#include<stdlib.h>

struct Node{
    int data;
    struct Node* left,*right;
};

struct Node* createNode(int data){
    struct Node* newNode=(struct Node*)malloc(sizeof(struct Node));
    newNode->data=data;
    newNode->left=NULL;
    newNode->right=NULL;

    return newNode;
}

struct Node* insert(struct Node *root,int data){
    if (root==NULL)
    {
        return createNode(data);
    }
    if (data<root->data)
    {
        root->left=insert(root->left,data);
    }else if(data>root->data){
        root->right=insert(root->right,data);
    }
    
   return root; 
}

void inOrder(struct Node* root){
    if (root==NULL)
    {
        return;
    }
    if (root!=NULL)
    {
        inOrder(root->left);
        printf("%d\t",root->data);
        inOrder(root->right);
    }
}

int main(){
    struct Node* root=NULL;

    root=insert(root,24);
    insert(root,12);
    insert(root,30);
    insert(root,55);
    insert(root,5);

    inOrder(root);
    return 0;
}