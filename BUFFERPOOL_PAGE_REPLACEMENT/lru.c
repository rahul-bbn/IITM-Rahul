#include<stdio.h>
#include<stdlib.h>
#include <stdio.h>

#define MAX_FRAMES 3
#define MAX_PAGES 12

int pages[MAX_PAGES] = {1,2,3,4,1,2,5,1,2,3,4,5};

int findpage(int buffer[], int page, int n){
    for (int i = 0; i < n; i++)
    {
        if (buffer[i] == page)
        {
            return i;
        }
    }
    return -1;
}

void printBuffer(int buffer[]){
    printf("Buffer: ");
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        if (buffer[i] == -1)
            printf("- ");
        else
            printf("%d ", buffer[i]);
    }
    printf("\n");
}

void simulateLRU(){
    int buffer[MAX_FRAMES];
    int time[MAX_FRAMES]; // keeps track of when each page was last used
    int hits = 0, misses = 0, counter = 0;

    for (int i = 0; i < MAX_FRAMES; i++)
    {
        buffer[i] = -1;
        time[i] = 0;
    }

    printf("=== LRU Page Replacement Simulation ===\n");

    // Process all pages
    for (int i = 0; i < MAX_PAGES; i++)
    {
        int page = pages[i];
        int pos = findpage(buffer, page, MAX_FRAMES);

        printf("Requesting page %d -> ", page);

        if (pos != -1)
        {
            hits++;
            time[pos] = ++counter; // update last used time
            printf("HIT\n");
        } else {
            // MISS
            misses++;

            int empty = -1;
            for (int j = 0; j < MAX_FRAMES; j++)
            {
                if (buffer[j] == -1)
                {
                    empty = j;                  
                    break;
                }
            }

            if (empty != -1)
            {
                buffer[empty] = page;
                time[empty] = ++counter;
            } else {
                // find least recently used (minimum time)
                int lru = 0;
                for (int j = 1; j < MAX_FRAMES; j++)
                {
                    if (time[j] < time[lru])
                        lru = j;
                }

                buffer[lru] = page;
                time[lru] = ++counter;
            }

            printf("MISS (Loaded %d into buffer)\n", page);
        }

        printBuffer(buffer);
    }

    printf("\n=== Simulation Summary ===\n");
    printf("Total Page Requests: %d\n", MAX_PAGES);
    printf("Total Hits: %d\n", hits);
    printf("Total Misses: %d\n", misses);
    printf("Hit Ratio: %.2f%%\n", (hits * 100.0) / MAX_PAGES);
    printf("Miss Ratio: %.2f%%\n", (misses * 100.0) / MAX_PAGES);
}

int main(){
    simulateLRU();
    return 0;
}