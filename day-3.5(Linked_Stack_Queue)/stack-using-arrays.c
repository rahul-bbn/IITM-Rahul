#include<stdio.h>
#include<stdbool.h>


#define SIZE 5

struct Stack {
    int array[SIZE];
    int top;
};

// init
void init(struct Stack *s)
{
    s->top = -1;
}

//isEmpty
bool isEmpty(struct Stack *s)
{
    return s->top == -1;
}

//isFull
bool isFull(struct Stack *s)
{
    return s->top == SIZE - 1;
}

//push
void push (struct Stack *s, int value)
{
    if(isFull(s))
    {
        printf("Stack overflow! Can't insert %d \n", value);
        return;
    }
    s->array[++(s->top)] = value;
    printf("%d is pushed to the stack \n", value);
}

//pop
int pop(struct Stack *s)
{
    if(isEmpty(s))
    {
        printf("Stack underflow! Can't pop \n");
        return -1;
    }
    return s->array[(s->top)--];
}

//peek
int peek(struct Stack *s)
{
    if(isEmpty(s))
    {
        printf("Stack is empty \n");
        return -1;
    }
    return s->array[s->top];
}

//display
void display(struct Stack *s)
{
    if(isEmpty(s))
    {
        printf("Stack is empty \n");
        return;
    }

    printf("Stack elements : \n");
    for (int iterator = s->top; iterator >=0; iterator--)
    {
        printf("%d \n", s->array[iterator]);
    }
    printf("\n");
}

int main()
{
    struct Stack s;

    init(&s);
    display(&s);

    push(&s, 10);
    display(&s);

    push(&s, 20);
    display(&s);

    push(&s, 30);
    display(&s);

    printf("Popped element : %d \n", pop(&s));
    display(&s);

    printf("Top element : %d \n", peek(&s));
    display(&s);

    return 0;
}