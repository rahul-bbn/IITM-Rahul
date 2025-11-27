#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

struct Node{
    int data;
    struct Node* left,*right;
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

int isBstUtils(struct Node* root,int min,int max){
    if (root==NULL)
    {
        return 1;
    }
    if (root->data<=min || root->data>=max)
    {
        return 0;
    }
    return isBstUtils(root->left,min,root->data) &&
           isBstUtils(root->right,root->data,max);
    
}

int isBST(struct Node* root){
    return isBstUtils(root,INT_MIN,INT_MAX);
}

void inorder(struct Node* root){
    if (root==NULL)
    {
        return;
    }
    if (root!=NULL)
    {
        inorder(root->left);
        printf("%d\t",root->data);
        inorder(root->right);
    }
}

int main(){
    struct Node* root=NULL;

    root=insert(root,24);
    insert(root,14);
    insert(root,28);
    insert(root,8);
    insert(root,15);

    inorder(root);

    if (isBST(root))
    {
        printf("\nValid BST");
    }else{
        printf("Invalid BST");
    }
    

    return 0;
}