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

int height(struct Node* root){
    if (root==NULL)
    {
        return 0;
    }
    int lh=height(root->left);
    int rh=height(root->right);

    return (lh>rh?lh:rh)+1;
    
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

    printf("\nHeigh of tree: %d",height(root));
    return 0;
}