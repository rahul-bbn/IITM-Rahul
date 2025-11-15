/*Write a C program that simulates a university student enrollment system using AVL Trees. Insert student records (RollNo + Name), support search by RollNo, and allow deletion when a student drops a course. Ensure the tree stays balanced after each operation using AVL rotations. Print all enrollment/drop events and the final sorted student list.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// AVL Tree Node
struct Node
{
    int rollNo;
    char name[20];
    int height;
    struct Node *left,*right;
};

int max(int a,int b)
{
    return (a>b)?a:b;
}

int height(struct Node *root)
{
    if (root==NULL)
    {
        return 0;
    }
    return root->height;
}

// create new student node
struct Node *createNode(int roll,char *name)
{
    struct Node *newNode=(struct Node*)malloc(sizeof(struct Node));

    newNode->rollNo=roll;
    strcpy(newNode->name,name);
    newNode->left=NULL;
    newNode->right=NULL;
    newNode->height=1;

    return newNode;
}

// left rotate
struct Node *leftRotate(struct Node *x)
{
    struct Node *y=x->right;
    struct Node *T2=y->left;

    y->left=x;
    x->right=T2;

    x->height=max(height(x->left),height(x->right))+1;
    y->height=max(height(y->left),height(y->right))+1;

    return y;
}

// right rotate
struct Node *rightRotate(struct Node *y)
{
    struct Node *x=y->left;
    struct Node *T2=x->right;

    x->right=y;
    y->left=T2;

    y->height=max(height(y->left),height(y->right))+1;
    x->height=max(height(x->left),height(x->right))+1;

    return x;
}

int getBalance(struct Node *n)
{
    if (n==NULL)
    {
        return 0;
    }
    return height(n->left)-height(n->right);
}

// Insert student
struct Node *insert(struct Node *root,int roll,char *name)
{
    if (root==NULL)
    {
        printf("Enrolled: %d  %s\n",roll,name);
        return createNode(roll,name);
    }
    if (roll<root->rollNo)
    {
        root->left=insert(root->left,roll,name);
    }
    else if (roll>root->rollNo)
    {
        root->right=insert(root->right,roll,name);
    }
    else
    {
        printf("Roll number already exists!\n");
        return root;
    }

    root->height=max(height(root->left),height(root->right))+1;
    int balance=getBalance(root);

    // Rotate
    if (balance>1 && roll<root->left->rollNo)
    {
        return rightRotate(root);
    }
    if (balance-1 && roll>root->right->rollNo)
    {
        return leftRotate(root);
    }
    if (balance>1 && roll>root->left->rollNo)
    {
        root->left=leftRotate(root->left);
        return rightRotate(root);
    }

    if (roll<-1 && roll<root->right->rollNo)
    {
        root->right=rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Find minimum node
struct Node *minValueNode(struct Node *node)
{
    struct Node *current=node;
    while (current->left!=NULL)
    {
        current=current->left;
    }
    return current;
}

// Delete student
struct Node *deleteNode(struct Node *root,int roll)
{
    if (root==NULL)
    {
        printf("Student not found!\n");
        return root;
    }
    if (roll<root->rollNo)
    {
        root->left=deleteNode(root->left,roll);
    }
    else if (roll>root->rollNo)
    {
        root->right=deleteNode(root->right,roll);
    }
    else
    {
        printf("Dropped: %d  %s\n",root->rollNo,root->name);

            if (root->left==NULL || root->right==NULL)
        {
            struct Node *temp=root->left?root->left:root->right;
            if (temp==NULL)
            {
                temp=root;
                root=NULL;
            }
            else
            {
                *root=*temp;
            }
            free(temp);
        }
        else
        {
            struct Node *succ=minValueNode(root->right);
            root->rollNo=succ->rollNo;
            strcpy(root->name,succ->name);
            root->right=deleteNode(root->right,succ->rollNo);
        }
    }
    if (root==NULL)
    {
        return root;
    }
    root->height=max(height(root->left),height(root->right))+1;
    int balance=getBalance(root);

    if (balance>1 && getBalance(root->left)>=0)
    {
        return rightRotate(root);
    }
    if (balance<-1 && getBalance(root->right)<=0)
    {
        return leftRotate(root);
    }
    
    if (balance>1 && getBalance(root->left)<0)
    {
        root->left=leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance<-1 && getBalance(root->right)>0)
    {
        root->right=rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

//search
void search(struct Node* root,int roll){
    if (root==NULL)
    {
        printf("Student not found!\n");
        return;
    }
    if (roll==root->rollNo)
    {
        printf("Found: %d  %s\n", root->rollNo, root->name);
    }
    else if (roll<root->rollNo)
    {
       search(root->left,roll);
    }
    else
       search(root->right,roll);   
    
}

// Inorder print
void inOrder(struct Node *root)
{
    if (root!=NULL)
    {
        inOrder(root->left);
        printf("%d %s\n",root->rollNo,root->name);
        inOrder(root->right);
    }
}

int main()
{
    struct Node *root=NULL;
    char name[20];
    int roll,choice;

    while (1)
    {
        printf("\n--- Student Enrollment System ---\n");
        printf("1. Enroll Student\n2. Drop Student\n3. Search Student\n4. Print All Students\n5. Exit\nEnter choice: ");
        scanf("%d",&choice);

        switch (choice)
        {
        case 1:
            printf("Enter student's roll number:");
            scanf("%d,",&roll);
            printf("Enter student's name:");
            scanf("%s",name);
            root = insert(root,roll,name);
            break;
        case 2:
            printf("Enter Roll Number to Drop: ");
            scanf("%d",&roll);
            root=deleteNode(root,roll);
            break;
        case 3:
            printf("Enter Roll Number to search: ");
            scanf("%d",&roll);
            search(root,roll);
            break;

        case 4:
            printf("---- Students data (in asc order (roll number))----\n");
            inOrder(root);
            break;
        case 5:
            printf("Exiting......");
            return 0;
        default:
            printf("Invalid choice! Try again.\n");
        }
    }
}