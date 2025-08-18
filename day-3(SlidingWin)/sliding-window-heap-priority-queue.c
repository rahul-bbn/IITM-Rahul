#include<stdio.h>
#include<stdlib.h>

typedef struct {
    int value; 
    int index;
}HeapNode;

void swap(HeapNode* nodeA, HeapNode* nodeB)
{
    HeapNode temp = *nodeA;
    *nodeA = *nodeB;
    *nodeB = temp;
}

void heapifyUp(HeapNode* heap, int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if(heap[parent].value >= heap[index].value) break;

        swap(&heap[parent], &heap[index]);
        index = parent;
    }
}

void heapifyDown(HeapNode* heap, int size, int index)
{
    while(1)
    {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if(left < size && heap[left].value > heap[largest].value)
        {
            largest = left;
        }

        if(right < size && heap[right].value > heap[largest].value)
        {
            largest = right;
        }

        if(largest == index) break;

        swap(&heap[index], &heap[largest]);
        index = largest;
    }
}

void push(HeapNode* heap, int* size, int index, int value)
{
    heap[*size].value = value;
    heap[*size].index = index;
    heapifyUp(heap, (*size)++);
}

void pop(HeapNode* heap, int* size)
{
    heap[0] = heap[--(*size)];
    heapifyDown(heap, *size, 0);
}

HeapNode top(HeapNode* heap)
{
    return heap[0];
}

void maximumSlidingWindow(int* nums, int numsSize, int k)
{
    HeapNode* heap = malloc(numsSize * sizeof(HeapNode));
    int heapSize = 0;

    for (int iterator = 0; iterator < numsSize; iterator++)
    {
        push(heap, &heapSize, iterator, nums[iterator]);

        while (top(heap).index <= iterator - k)
        {
            pop(heap, &heapSize);
        }

        if(iterator >= k - 1) printf("%d", top(heap).value);
    }
    printf("\n");
    free(heap);
}

int main()
{
    int nums[] = {1,3,-1,-3,5,3,6,7};
    int k = 3;
    int n = sizeof(nums)/sizeof(nums[0]);

    maximumSlidingWindow(nums, n, k);
    return 0;
}

// Time Complexity - O(n log k)


