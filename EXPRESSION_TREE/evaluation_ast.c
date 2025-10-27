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

int evaluation(struct Node* root){
    if (root==NULL)
    {
        return 0;
    }
    if (!root->left && !root->right)
    {
        return atoi(root->data);
    }
    
    int leftval=evaluation(root->left);
    int rightval=evaluation(root->right);

    if (strcmp(root->data,"+")==0)
    {
        return leftval+rightval;
    }else if(strcmp(root->data,"-")==0)
    {
        return leftval-rightval;
    }else if (strcmp(root->data,"*")==0)
    {
        return leftval*rightval;
    }else if (strcmp(root->data,"/")==0)
    {
        if (rightval==0)
        {
            return 0;
        }else{
            return leftval/rightval;
        }
    }
    
    return 0;
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

    //(5+3)*(8-2)
    struct Node* root=NULL;
    root=createNode("*");

    root->left=createNode("+");
    root->left->left=createNode("5");
    root->left->right=createNode("3");

    root->right=createNode("-");
    root->right->left=createNode("8");
    root->right->right=createNode("2");

    printf("preOrde traversal:\n");
    preOrder(root);

    printf("\npostOrde traversal:\n");
    postOrder(root);

    printf("\n\nResult of expression: %d\n", evaluation(root));

    return 0;
}