#include<stdio.h>
#include<stdbool.h>

#define SIZE 5

struct Queue {
    int array[SIZE];
    int front, rear;
};

// init
void init(struct Queue *q)
{
    q->front = -1;
    q->rear = -1;
}

// isEmpty
bool isEmpty(struct Queue *q)
{
    return q->front == -1;
}

// isFull
bool isFull(struct Queue *q)
{
    return q->rear == SIZE - 1;
}

//enqueue
void enqueue(struct Queue *q, int value)
{
    if(isFull(q))
    {
        printf("Queue overflow! \n");
        return;
    }

    if(q->front == -1) q->front=0;
    q->array[++(q->rear)] = value;
    printf("%d is added to the queue \n", value);
}

//dequeue
int dequeue(struct Queue *q)
{
    if(isEmpty(q))
    {
        printf("Queue underflow! \n");
        return -1;
    }

    int data = q->array[q->front];
    if(q->front == q->rear)
    {
        q->front = q->rear = -1;
    }
    else
    {
        q->front++;
    }
    return data;
}

//display
void display(struct Queue *q)
{
    if(isEmpty(q))
    {
        printf("Queue is empty! \n");
        return;
    }

    printf("Queue elements : \n");
    for (int iterator = q->front; iterator<= q->rear; iterator++)
    {
        printf("%d ",q->array[iterator]);
    }
    printf("\n");
}

int main()
{
    struct Queue q;

    init(&q);
    display(&q);

    enqueue(&q, 10);
    display(&q);

    enqueue(&q, 20);
    display(&q);

    enqueue(&q, 30);
    display(&q);

    printf("Dequeued number : %d \n", dequeue(&q));
    display(&q);

    return 0;
}