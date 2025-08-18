#include<stdio.h>
#include<stdlib.h>

void executeHeap()
{
    int *dynamicArray = malloc(1000 * sizeof(int));  //Heap memory is allocated

    if(!dynamicArray)
    {
        fprintf(stderr, "Memory Allocation failed");
        return;
    }

    for (int iterator = 0; iterator < 1000; iterator++)
    {
        dynamicArray[iterator] = iterator * 2;  //Allocated memory is used
    }

    free(dynamicArray);     //The allocated memory is unallocated
    dynamicArray = NULL;
}

int main()
{
    executeHeap();
    return 0;
}

// Dynamic memory allocation
// Flexible Memory Size (RAM capacity)
// Managed manually
// Comparably slower in allocation and deallocation (involves system calls)
// Fragmentation is a con