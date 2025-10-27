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

    //arithmetic
    if (strcmp(root->data,"+")==0)
    {
        return leftval+rightval;
    }
    if(strcmp(root->data,"-")==0)
    {
        return leftval-rightval;
    }
    if (strcmp(root->data,"*")==0)
    {
        return rightval;
    }
    if (strcmp(root->data,"/")==0)
    {
        if (rightval==0)
        {
            return 0;
        }else{
            return leftval/rightval;
        }
        
    }

    //comparision
    if (strcmp(root->data,">")==0)
    {
        return leftval>rightval;
    }
    if (strcmp(root->data,"<")==0)
    {
       return leftval<rightval;
    }
    if (strcmp(root->data,"=")==0)
    {
        return leftval==rightval;
    }
    if (strcmp(root->data,">=")==0)
    {
       return leftval>=rightval;
    }
    if (strcmp(root->data,"<=")==0)
    {
        return leftval<=rightval;
    }
    if (strcmp(root->data,"!=")==0)
    {
        return leftval!=rightval;
    }
    
    //logical
    if (strcmp(root->data,"AND")==0)
    {
        return leftval && rightval;
    }
    if (strcmp(root->data,"OR")==0)
    {
        return leftval || rightval;
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

    /*
    SQL-like Expression:
        ((5 + 3) > (8 - 2)) AND (4 = 4)

        AST:
                    AND
                  /     \
                >         =
              /   \     /   \
            +       -  4     4
           / \     / \
          5   3   8   2
    */
    struct Node* root=NULL;
    root=createNode("AND");

    root->left=createNode(">");
    root->left->left=createNode("+");
    root->left->left->left=createNode("5");
    root->left->left->right=createNode("3");

    root->left->right=createNode("-");
    root->left->right->left=createNode("8");
    root->left->right->right=createNode("8");

    root->right=createNode("=");
    root->right->left=createNode("4");
    root->right->right=createNode("4");

    printf("preOrde traversal:\n");
    preOrder(root);

    printf("\npostOrde traversal:\n");
    postOrder(root);

    printf("\n\nResult of expression: %d\n", evaluation(root));

    return 0;
}