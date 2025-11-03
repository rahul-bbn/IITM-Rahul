#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 3
#define MAX_PAGES 12
int pages[MAX_PAGES] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};

//LRU
void simulateLRU()
{
    int buffer[MAX_FRAMES];
    int time[MAX_FRAMES];
    int hits = 0, misses = 0, counter = 0;

    for (int i = 0; i < MAX_FRAMES; i++)
    {
        buffer[i] = -1;
        time[i] = 0;
    }

    printf("\n===== LRU Page Replacement =====\n\n");

    for (int i = 0; i < MAX_PAGES; i++)
    {
        int page = pages[i];
        int found = -1;

        for (int j = 0; j < MAX_FRAMES; j++)
            if (buffer[j] == page)
                found = j;

        if (found != -1)
        {
            hits++;
            time[found] = ++counter;
        }
        else
        {
            misses++;
            int empty = -1;
            for (int j = 0; j < MAX_FRAMES; j++)
                if (buffer[j] == -1)
                    empty = j;

            if (empty != -1)
            {
                buffer[empty] = page;
                time[empty] = ++counter;
            }
            else
            {
                int lru = 0;
                for (int j = 1; j < MAX_FRAMES; j++)
                    if (time[j] < time[lru])
                        lru = j;
                buffer[lru] = page;
                time[lru] = ++counter;
            }
        }

        printf("Step %2d (Page %d): ", i + 1, page);
        for (int j = 0; j < MAX_FRAMES; j++)
            buffer[j] == -1 ? printf("- ") : printf("%d ", buffer[j]);
        printf(found != -1 ? "-> HIT\n" : "-> MISS\n");
    }

    printf("\nTotal Hits: %d\nTotal Misses: %d\nHit Ratio: %.2f\n", hits, misses, (float)hits / MAX_PAGES);
}

//CLOCK
void simulateClock()
{
    int buffer[MAX_FRAMES];
    int refBit[MAX_FRAMES];
    int pointer = 0, hits = 0, misses = 0;

    for (int i = 0; i < MAX_FRAMES; i++)
    {
        buffer[i] = -1;
        refBit[i] = 0;
    }

    printf("\n===== CLOCK Page Replacement =====\n\n");

    for (int i = 0; i < MAX_PAGES; i++)
    {
        int page = pages[i];
        int found = 0;

        for (int j = 0; j < MAX_FRAMES; j++)
        {
            if (buffer[j] == page)
            {
                found = 1;
                refBit[j] = 1;
                hits++;
                break;
            }
        }

        if (!found)
        {
            misses++;
            while (1)
            {
                if (refBit[pointer] == 0)
                {
                    buffer[pointer] = page;
                    refBit[pointer] = 1;
                    pointer = (pointer + 1) % MAX_FRAMES;
                    break;
                }
                else
                {
                    refBit[pointer] = 0;
                    pointer = (pointer + 1) % MAX_FRAMES;
                }
            }
        }

        printf("Step %2d (Page %d): ", i + 1, page);
        for (int k = 0; k < MAX_FRAMES; k++)
            buffer[k] == -1 ? printf("- ") : printf("%d ", buffer[k]);
        printf(found ? "-> HIT\n" : "-> MISS\n");
    }

    printf("\nTotal Hits: %d\nTotal Misses: %d\nHit Ratio: %.2f\n", hits, misses, (float)hits / MAX_PAGES);
}

// --------------------- ARC (Simplified) ---------------------
// This is a *simplified* Adaptive Replacement Cache version (conceptual for learning)
typedef struct
{
    int T1[MAX_FRAMES]; // recently used
    int T2[MAX_FRAMES]; // frequently used
    int B1[MAX_FRAMES]; // ghost for T1
    int B2[MAX_FRAMES]; // ghost for T2
    int t1Size, t2Size, b1Size, b2Size;
    int hits, misses;
} ARC;

int contains(int arr[], int size, int page)
{
    for (int i = 0; i < size; i++)
        if (arr[i] == page)
            return 1;
    return 0;
}

void removeFirst(int arr[], int *size)
{
    for (int i = 1; i < *size; i++)
        arr[i - 1] = arr[i];
    (*size)--;
}

void addLast(int arr[], int *size, int page)
{
    arr[(*size)++] = page;
}

void printARC(ARC *arc)
{
    printf("T1: ");
    for (int i = 0; i < arc->t1Size; i++)
        printf("%d ", arc->T1[i]);
    printf("| T2: ");
    for (int i = 0; i < arc->t2Size; i++)
        printf("%d ", arc->T2[i]);
    printf("\n");
}

void simulateARC()
{
    ARC arc = {.t1Size = 0, .t2Size = 0, .b1Size = 0, .b2Size = 0, .hits = 0, .misses = 0};

    printf("\n===== ARC Page Replacement (Simplified) =====\n\n");

    for (int i = 0; i < MAX_PAGES; i++)
    {
        int page = pages[i];

        // HIT in T1 or T2
        if (contains(arc.T1, arc.t1Size, page) || contains(arc.T2, arc.t2Size, page))
        {
            arc.hits++;
            if (contains(arc.T1, arc.t1Size, page))
            {
                // Move from T1 to T2
                for (int j = 0; j < arc.t1Size; j++)
                {
                    if (arc.T1[j] == page)
                    {
                        for (int k = j; k < arc.t1Size - 1; k++)
                            arc.T1[k] = arc.T1[k + 1];
                        arc.t1Size--;
                        break;
                    }
                }
                addLast(arc.T2, &arc.t2Size, page);
            }
            printf("Step %2d (Page %d): HIT | ", i + 1, page);
            printARC(&arc);
        }
        else
        {
            arc.misses++;
            // MISS
            if (arc.t1Size + arc.b1Size == MAX_FRAMES)
            {
                if (arc.t1Size < MAX_FRAMES)
                {
                    removeFirst(arc.B1, &arc.b1Size);
                }
                else
                {
                    removeFirst(arc.T1, &arc.t1Size);
                }
            }

            if (arc.t1Size + arc.t2Size < MAX_FRAMES)
            {
                addLast(arc.T1, &arc.t1Size, page);
            }
            else
            {
                if (arc.t2Size < MAX_FRAMES)
                {
                    removeFirst(arc.T2, &arc.t2Size);
                    addLast(arc.T1, &arc.t1Size, page);
                }
                else
                {
                    removeFirst(arc.T1, &arc.t1Size);
                    addLast(arc.T1, &arc.t1Size, page);
                }
            }
            printf("Step %2d (Page %d): MISS | ", i + 1, page);
            printARC(&arc);
        }
    }

    printf("\nTotal Hits: %d\nTotal Misses: %d\nHit Ratio: %.2f\n", arc.hits, arc.misses, (float)arc.hits / MAX_PAGES);
}

int main()
{
    simulateLRU();
    simulateClock();
    simulateARC();
    return 0;
}