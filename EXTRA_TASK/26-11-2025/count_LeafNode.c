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

int countLeafNode(struct Node* root){
    if (root==NULL)
    {
        return 0;
    }
    
    if (root->left==NULL && root->right==NULL)
    {
        return 1;
    }
    return countLeafNode(root->left)+countLeafNode(root->right);
    
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
    insert(root,19);

    inOrder(root);

    printf("\nTotal node in bst: %d",countLeafNode(root));
    return 0;
}