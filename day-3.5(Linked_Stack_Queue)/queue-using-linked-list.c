#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct Queue {
    struct Node *front;
    struct Node *rear;
};

// create node
struct Node* createNode(int value) {
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

// init
void init(struct Queue *q) {
    q->front = NULL;
    q->rear = NULL;
}

// isEmpty
int isEmpty(struct Queue *q) {
    return q->front == NULL;
}

// enqueue
void enqueue(struct Queue *q, int value) {
    struct Node *newNode = createNode(value);
    if (q->rear == NULL) { 
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode; 
        q->rear = newNode;       
    }
    printf("%d enqueued to queue\n", value);
}

// dequeue
int dequeue(struct Queue *q) {
    if (isEmpty(q)) {
        printf("Queue Underflow! Nothing to dequeue\n");
        return -1;
    }
    struct Node *temp = q->front;
    int value = temp->data;
    q->front = q->front->next;

    if (q->front == NULL) { 
        q->rear = NULL;
    }
    free(temp);
    return value;
}

// peek
int peek(struct Queue *q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return -1;
    }
    return q->front->data;
}

// display
void display(struct Queue *q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue elements: ");
    struct Node *temp = q->front;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int main() {
    struct Queue q;
    init(&q);
    display(&q);

    enqueue(&q, 10);
    display(&q);

    enqueue(&q, 20);
    display(&q);

    enqueue(&q, 30);
    display(&q);

    printf("Dequeued element: %d \n", dequeue(&q));
    display(&q);

    printf("Front element: %d \n", peek(&q));

    return 0;
}
