/*Write a C program that simulates a university student enrollment system using AVL Trees. Insert student records (RollNo + Name), support search by RollNo, and allow deletion when a student drops a course. Ensure the tree stays balanced after each operation using AVL rotations. Print all enrollment/drop events and the final sorted student list.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Node{
    int rollNo;
    char name[20];
    int height;
    struct Node* left,*right;
};

struct Node* createNode(int roll, char *name){
    struct Node* newNode=(struct Node*)malloc(sizeof(struct Node));

    newNode->rollNo=roll;
    strcpy(newNode->name, name);
    newNode->left=NULL;
    newNode->right=NULL;
    newNode->height=1;

    return newNode;
}

struct Node* insert(struct Node* root,int roll, char * name){
    if (root==NULL)
    {
        return createNode(roll,name);
    }
    if (roll<root->rollNo)
    {
        root->left=insert(root->left,roll,name);
    }else if (roll>root->rollNo)
    {
        root->right=insert(root->right,roll,name);
    }
    
    return root;
}

void inOrder(struct Node* root){
    if (root!=NULL)
    {
        inOrder(root->left);
        printf("%d %s",root->rollNo,root->name);
        inOrder(root->right);
    }
    
}

int main(){
    struct Node* root=NULL;
    char name[20];
    int roll;

    printf("Enter name and roll number:");
    scanf("%d,",&roll);
    scanf("%s",name);
    
    root=insert(root,roll,name);

    inOrder(root);

    return 0;
}

