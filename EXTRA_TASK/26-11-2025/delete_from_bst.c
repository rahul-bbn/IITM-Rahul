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
    }else if(val>root->data){
        root->right=insert(root->right,val);
    }
    return root;
}

struct Node* deleteNode(struct Node* root,int val){
    if (root==NULL)
    {
        return root;
    }
    if (val<root->data)
    {
       root->left=deleteNode(root->left,val);
    }else if (val>root->data)
    {
        root->right=deleteNode(root->right,val);
    }else{
        if (root->left==NULL && root->right==NULL)
        {
            free(root);
            return NULL;
        }else if (root->left==NULL)
        {
            struct Node* temp=root->right;
            free(root);
            return temp;
        }else if (root->right==NULL)
        {
            struct Node* temp=root->left;
            free(root);
            return temp;
        }
        
        struct Node* temp=root->right;
        while (temp->left!=NULL)
        {
            temp=temp->left;
        }
        root->data=temp->data;
        root->right=deleteNode(root->right,temp->data);
        
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

    root=insert(root,15);
    insert(root,9);
    insert(root,25);
    insert(root,5);
    insert(root,11);
    insert(root,18);

    inOrder(root);

    deleteNode(root,9);
    printf("\nAfter deleting node, The resultant Tree is:\n");

    inOrder(root);

    return 0;
}