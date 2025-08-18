#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>

#define POOL_SIZE 1024

typedef struct Block {
    size_t size;
    struct Block* next;
}Block;

static uint8_t memory_pool[POOL_SIZE];
static Block* free_list = NULL;

//pool init
void pool_init()
{
    free_list = (Block*)memory_pool;
    free_list->size = POOL_SIZE - sizeof(Block);
    free_list->next = NULL;
}

//pool allocate
void* pool_allocate(size_t size)
{
    Block* prev = NULL;
    Block* current = free_list;

    while (current)
    {
        if(current->size >= size)
        {
            if(current-> size >= size + sizeof(Block) + 1)
            {
                Block* new_block = (Block*)((uint8_t*)current + sizeof(Block) + size);
                new_block->size = current->size - size - sizeof(Block);
                new_block->next = current->next;

                current->size = size;

                if(prev)
                {
                    prev->next = new_block;
                }
                else
                {
                    free_list = new_block;
                }
            }
            else
            {
                if(prev)
                {
                    prev->next = current->next;
                }
                else
                {
                    free_list = current->next;
                }
            }

            printf("Allocation of %zu bytes is successful at %p \n", size, (void*)((uint8_t*)current + sizeof(Block)));
            return (uint8_t*)current + sizeof(Block);
        }

        prev = current;
        current = current->next;
    }

    printf("Allocation of %zu bytes is failed", size);
    return NULL;
}

//pool free
void pool_free(void* ptr)
{
    if(!ptr)
    {
        return;
    }

    Block* block_to_free = (Block*)((uint8_t*)ptr - sizeof(Block));
    block_to_free->next = free_list;
    free_list = block_to_free;

    printf("%zu bytes of data is freed from %p \n", block_to_free->size, ptr);
}

int main()
{
    pool_init();
    void* ptr1 = pool_allocate(256);
    void* ptr2 = pool_allocate(128);
    pool_free(ptr1);
    void* ptr3 = pool_allocate(512);
 
    return 0;
}




// Sample Input:
// pool_allocate(256); pool_allocate(128); pool_free(ptr1); pool_allocate(512);
 
// Sample Output:
// Allocated 256 bytes at ptr1
// Allocated 128 bytes at ptr2
// Freed 256 bytes at ptr1
// Allocated 512 bytes at ptr3