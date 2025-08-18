#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Queue {
    struct Node** items;
    int front, rear, size;
};

//createNode
struct Node* createNode(int value)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));

    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

//createQueue
struct Queue* createQueue(int capacity)
{
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->items = (struct Node**)malloc(capacity * sizeof(struct Node*));
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;

    return queue;
}

//isEmpty
bool isEmpty(struct Queue* queue)
{
    return queue->size == 0;
}

//enqueue
void enqueue(struct Queue* queue, struct Node* node)
{
    queue->rear++;
    queue->items[queue->rear] = node;
    queue->size++;
}

//dequeue
struct Node* dequeue(struct Queue* queue)
{
    struct Node* node = queue->items[queue->front];
    queue->front++;
    queue->size--;

    return node;
}

//insertNode
struct Node* insertNode(struct Node* root, int value)
{
    struct Node* newNode = createNode(value);

    if(root == NULL)
    {
        return newNode;
    }

    struct Queue* queue = createQueue(100);
    enqueue(queue, root);

    while(!isEmpty(queue))
    {
        struct Node* temp = dequeue(queue);

        if(temp->left == NULL)
        {
            temp->left = newNode;
            break;
        }
        else
        {
            enqueue(queue, temp->left);
        }

        if(temp->right == NULL)
        {
            temp->right = newNode;
            break;
        }
        else
        {
            enqueue(queue, temp->right);
        }
    }

    free(queue->items);
    free(queue);

    return root;
}

//inorder - Left->Root->Right
void inorder(struct Node* root)
{
    if(root == NULL)
    {
        return ;
    }
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

//preorder - Root->Left->Right
void preorder(struct Node* root)
{
    if(root == NULL)
    {
        return ;
    }

    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

//postorder - Left->Right->Root
void postorder(struct Node* root)
{
    if(root == NULL)
    {
        return ;
    }

    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

int main()
{
    struct Node* root = NULL;
    int choice, value;

    while(1)
    {
        printf("\n------ Binary Tree Handling ----- \n");
        printf("1. Insert a node \n");
        printf("2. Inorder Traversal \n");
        printf("3. Preorder Traversal \n");
        printf("4. Postorder Traversal \n");
        printf("5. Exit \n");
        printf("Enter your choice : ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter the value to insert : ");
            scanf("%d", &value);
            printf("\n");
            root = insertNode(root, value);
            printf("Inserted %d successfully", value);
            break;

        case 2:
            printf("InOrder Traversal : \n");
            inorder(root);
            printf("\n");
            break;
        
        case 3:
            printf("PreOrder Traversal : \n");
            preorder(root);
            printf("\n");
            break;
        
        case 4:
            printf("PostOrder Traversal : \n");
            postorder(root);
            printf("\n");
            break;

        case 5:
            exit(0);
        
        default:
            printf("Invalid choice. Please try again \n");
        }
    }

    return 0;
}