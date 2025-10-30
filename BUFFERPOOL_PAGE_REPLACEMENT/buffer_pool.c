#include<stdio.h>
#include<stdlib.h>

#define MAAX_PAGES 10
int buffer[MAAX_PAGES];
int buffer_size;

int isPageInBuffer(int page){
    for (int i = 0; i < buffer_size; i++){
        if (buffer[i]==page)
        {
            return 1;
        }
    }
    return 0;
}

void printBuffer(){
    printf("Buffer elements\n");
    for (int i = 0; i < buffer_size; i++)
    {
        if (buffer[i]==-1)
           printf("- ");
        else 
           printf("%d ", buffer[i]);
    }
    printf("\n");
    
}

int main(){
    int n,pages[20];

    printf("Enter number of pages:");
    scanf("%d",&n);

    printf("Enter %d pages into the pool:\n",n);
    for (int i = 0; i < n; i++)
    {
       scanf("%d",&pages[i]);
    }

    printf("Enter bufferpool size:");
    scanf("%d",&buffer_size);

    int nextInsert = 0;

    for(int i = 0; i < buffer_size; i++){
        buffer[i] = -1; 
    }
    
    for(int i = 0; i < n; i++){
        int page = pages[i];
        printf("Requesting page %d->",page);

        if(isPageInBuffer(page))
        {
            printf("HIT\n");
        } else{
            printf("MISS (Loading into buffer)\n");
            buffer[nextInsert] = page;
            nextInsert = (nextInsert+1) % buffer_size;
        } 
        printBuffer();
    }
    return 0;
}
