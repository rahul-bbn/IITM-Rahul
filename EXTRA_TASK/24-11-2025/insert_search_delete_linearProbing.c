#include<stdio.h>
#define TABLE_SIZE 10
#define EMPTY -1
#define DELETED -2

struct entry{
    int key;
    int value;
};

struct entry hashTable[TABLE_SIZE];

int hashFunction(int key){
    return key%TABLE_SIZE;
}

void initTable(){
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        hashTable[i].key=EMPTY;
        hashTable[i].value=0;
    }
}

void insert(int key,int value){
    int index=hashFunction(key);

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        int newIndex=(index+i)%TABLE_SIZE;
        if (hashTable[newIndex].key==EMPTY || hashTable[newIndex].value==DELETED)
        {
            hashTable[newIndex].key=key;
            hashTable[newIndex].value=value;
            printf("inserted (%d %d) at index %d\n",key,value,newIndex);
            return;
        }
    }
    printf("Not inserted!! hash is full!!");
}

int search(int key){
    int index=hashFunction(key);

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        int newIndex=(index+i)%TABLE_SIZE;
        if (hashTable[newIndex].key==-1)
        {
           return -1;
        }
        if (hashTable[newIndex].key==key)
        {
            return hashTable[newIndex].value;
        }
    }
    return -1; 
}

void delete(int key){
    int index=hashFunction(key);

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        int newIndex=(index+i)%TABLE_SIZE;
        if (hashTable[newIndex].key==EMPTY)
        {
            return;
        }
        if (hashTable[newIndex].key==key)
        {
            hashTable[newIndex].key=DELETED;
            printf("Deleted key %d from index %d\n", key, newIndex);
            return;
        }
        
        
    }
    
}

void display(){
    printf("\nHash Table:\n");
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (hashTable[i].key>=0)
        {
            printf("Index %d -> (key = %d and value = %d)\n",i,hashTable[i].key,hashTable[i].value);
        }else{
            printf("Index %d -> EMPTY\n",i);
        }
    }
    
}

int main(){

    initTable();

    insert(5,50);
    insert(15,150);
    insert(25,250);

    display();

    int val=search(15);
    if (val!=-1)
    {
        printf("\nfound value=%d\n",val);
    }else{
        printf("\nKey not found\n");
    }

    delete(15);
    display();
    
    return 0;
}