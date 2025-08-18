#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node {
    int data;
    struct Node *next;
};

//create a node
struct Node* createNode(int value)
{
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));

    if(!newNode)
    {
        printf("Memory allocation failed \n");
        exit(1);
    }

    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

//isEmpty
bool isEmpty(struct Node *top)
{
    return top == NULL;
}

//push (insert at head)
void push(struct Node **top, int value)
{
    struct Node *newNode = createNode(value);
    newNode->next = *top;
    *top = newNode;
    printf("%d is pushed to the stack \n", value);
}

//pop (remove from head)
int pop(struct Node **top)
{
    if(isEmpty(*top))
    {
        printf("Stack is underflow \n");
        return -1;
    }

    struct Node *temp = *top;
    int poppedValue = temp->data;
    *top = (*top)->next;
    free(temp);
    return poppedValue;
}

//peek (return the head)
int peek(struct Node *top)
{
    if(isEmpty(top))
    {
        printf("Stack is empty \n");
        return -1;
    }
    return top->data;
}


// display
void display(struct Node *top)
{
    if(isEmpty(top))
    {
        printf("Stack is empty \n");
        return;
    }

    printf("Stack elements : \n");
    struct Node *temp = top;
    while (temp != NULL)
    {
        printf("%d \n", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int main() 
{
    struct Node *top = NULL;
    display(top);

    push(&top, 10);
    display(top);

    push(&top, 20);
    display(top);

    push(&top, 30);
    display(top);

    printf("Popped Element is %d \n", pop(&top));
    display(top);

    printf("Top element is %d \n", peek(top));
    display(top);

    return 0;
}