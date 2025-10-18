//Implement a Binary Search Tree in C with insert and search operations. After insertion, print pre-order traversal of the tree

#include<stdio.h>
#include<stdlib.h>

struct Node{
    int data;
    struct Node *left;
    struct Node *right;
};

struct Node* createNode(int val){
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->data=val;
    newnode->left=NULL;
    newnode->right=NULL;

    return newnode;
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

struct Node* search(struct Node* root,int key){
    if (root==NULL || root->data==key)
    {
        return root;
    }else if (key<root->data)
    {
       return search(root->left,key);
    }else if (key>root->data)
    {
        return search(root->right,key);
    }
}

void pre_order(struct Node* root){
    if (root==NULL)
    {
        return;
    }
    printf("%d-> ",root->data);
    pre_order(root->left);
    pre_order(root->right);
    
}

int main(){
    struct Node* root=NULL;

    root=insert(root,10);
    insert(root,15);
    insert(root,5);
    insert(root,11);
    insert(root,7);

    int search_val;
    printf("Enter any number to search:");
    scanf("%d",&search_val);

    if (search(root,search_val)!=NULL)
    {
        printf("%d found in the BST.\n", search_val);
    }else{
        printf("%d not found in the BST.\n", search_val);
    }
    

    pre_order(root);
    printf("NULL\n");

    return 0;
}