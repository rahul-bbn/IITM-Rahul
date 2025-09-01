#include<stdio.h>
#include<string.h>

// 1. Defining Bit Array
#define BLOOM_SIZE 20
int bloom_filter[BLOOM_SIZE]; //all intialize to 0

void printBloom(){
    for (int i = 0; i < BLOOM_SIZE; i++)
    {
        printf("%d ",bloom_filter[i]);
    }
    printf("\n");
}

int hashFunone(char *word){
    // Sum of ASCII values of characters in the word
    int sum=0;
    for (int i = 0; word[i]!='\0'; i++)
    {
        sum +=word[i];
    }
    return sum%BLOOM_SIZE;
}

int hashFuntwo(char *word){
    //weighted sum of ASCII values of characters in the word
    int sum=0;
    for (int i = 0; word[i]!='\0'; i++)
    {
        sum+=(i+1)*word[i];
    }
    return sum%BLOOM_SIZE;
}

void insert(char *word){
    int firstIndex= hashFunone(word);
    int secondIndex= hashFuntwo(word);

    bloom_filter[firstIndex]=1;
    bloom_filter[secondIndex]=1;

    printf("inserted %s at indices %d and %d\n",word,firstIndex,secondIndex);
    printBloom();
}

int search(char *word){
    int firstIndex=hashFunone(word);
    int secondIndex=hashFuntwo(word);

    if(bloom_filter[firstIndex]==1 && bloom_filter[secondIndex]==1){
        printf("\"%s\" is possibly present at position %d %d\n",word,firstIndex,secondIndex);
        return 1;
    }else{
        printf("\"%s\" Definitely not present at position %d %d\n",word,firstIndex,secondIndex);
        return 0;
    }
}

int main(){
    memset(bloom_filter,0,sizeof(bloom_filter));

    insert("cat");
    insert("dog");
    insert("rat");
    insert("bat");

    search("cat");
    search("cow");

    // force a false positive
    printf("After forcefully false position:\n");
    bloom_filter[9] = 1;
    bloom_filter[18] = 1;
    search("cow");

    return 0;
}