#include<stdio.h>

int main(){
    char str[50];
    int freq[256]={0};
    
    scanf("%[^\n]s",str);
    for (int i = 0; str[i]; i++)
    {
        freq[str[i]]++;
    }
    for (int i = 0; i < 256; i++)
    {
        if (freq[i]!=0)
        {
            printf("%c -> %d\n",i,freq[i]);
        }
        
    }
    return 0;
}