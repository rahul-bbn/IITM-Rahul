#include<stdio.h>
#include<stdlib.h>

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

struct Node* LCA(struct Node* root,int a,int b){
    if (root==NULL)
    {
        return NULL;
    }
    if (a<root->data && b<root->data)
    {
        return LCA(root->left,a,b);
    }
    if (a>root->data && b>root->data)
    {
        return LCA(root->right,a,b);
    }
    return root;
}

void inorder(struct Node* root){
    if (root==NULL)
    {
        return;
    }
    if (root!=NULL)
    {
        inorder(root->left);
    }
    
    
}

int main(){
    struct Node* root=NULL;
    root=insert(root,10);
    insert(root,5);
    insert(root,20);
    insert(root,15);
    insert(root,30);

    printf("lowest common ancestor= %d",LCA(root,15,30)->data);

    return 0;
}