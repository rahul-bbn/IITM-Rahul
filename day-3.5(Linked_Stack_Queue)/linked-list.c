#include<stdio.h>
#include<stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

//create a node
struct Node* createNode(int value)
{
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

//insert - at beginning
void insertAtBeginning(struct Node **head, int value)
{
    struct Node *newNode = createNode(value);
    newNode->next = *head;
    *head = newNode;
}

//insert - at end
void insertAtEnd(struct Node **head, int value)
{
    struct Node *newNode = createNode(value);
    if(*head == NULL)
    {
        *head = newNode;
        return;
    }
    struct Node *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
}

//delete
void deleteNode (struct Node **head, int value)
{
    struct Node *temp = *head, *prev = NULL;

    if(temp != NULL && temp->data == value)
    {
        *head = temp->next;
        free(temp);
        return;
    }

    while(temp != NULL && temp->data != value)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    prev->next = temp->next;
    free(temp);
}

//display
void display(struct Node *head)
{
    struct Node *temp = head;
    while (temp != NULL)
    {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL \n");
}

int main() {
    struct Node *head = NULL;
    display(head);

    insertAtEnd(&head, 20);
    display(head);

    insertAtEnd(&head, 10);
    display(head);

    insertAtBeginning(&head, 5);
    display(head);
    
    deleteNode(&head, 20);
    display(head);

    return 0;
}