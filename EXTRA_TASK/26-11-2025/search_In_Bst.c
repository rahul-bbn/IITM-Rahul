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

int search(struct Node* root,int val){
    if (root==NULL)
    {
        return 0;
    }
    if (val==root->data)
    {
        return 1;
    }else if (val<root->data)
    {
        return search(root->left,val);
    }else if (val>root->data)
    {
        return search(root->right,val);
    }
}

int main(){
    struct Node* root=NULL;

    root=insert(root,15);
    insert(root,9);
    insert(root,25);
    insert(root,5);
    insert(root,11);
    insert(root,18);

    inorder(root);

    if (search(root,5))
    {
        printf("\nFound\n");
    }else{
        printf("\nNot Found\n");
    }
    

    return 0;
}