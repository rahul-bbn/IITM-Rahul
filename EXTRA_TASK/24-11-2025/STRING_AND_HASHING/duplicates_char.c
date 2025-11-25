#include<stdio.h>

int main(){
    char str[50];
    int freq[256]={0};

    printf("Enter string:");
    scanf("%[^\n]s",str);

    for (int i = 0; str[i]!='\0'; i++)
    {
        freq[str[i]]++;
    }
    for (int i = 0; i < 256; i++)
    {
        if(freq[i]>1){
            printf("%c -> %d times\n", i, freq[i]);
        }
    }
    return 0;
}