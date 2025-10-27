//Q. age > 20 AND marks < 80
/*
          AND
         /   \
       >       <
     /  \     /  \
   age  20  marks 80
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Node{
    char data[100];
    struct Node* left;
    struct Node* right;
};

struct Node* createNode(char *val){
    struct Node* newNode=(struct Node*)malloc(sizeof(struct Node));
    
    strcpy(newNode->data,val);
    newNode->left=newNode->right=NULL;

    return newNode;
}

void preOrder(struct Node* root){
    if (root==NULL)
    {
        return;
    }
    printf("%s ",root->data);
    preOrder(root->left);
    preOrder(root->right);
}

void postOrder(struct Node* root){
    if (root==NULL)
    {
        return;
    }
    postOrder(root->left);
    postOrder(root->right);
    printf("%s ",root->data);
}

int main(){
    struct Node* root=NULL;
    root=createNode("AND");

    root->left=createNode(">");
    root->left->left=createNode("age");
    root->left->right=createNode("20");

    root->right=createNode("<");
    root->right->left=createNode("marks");
    root->right->right=createNode("80");

    printf("preOrde traversal:\n");
    preOrder(root);

    printf("\npostOrde traversal:\n");
    postOrder(root);

    return 0;
}