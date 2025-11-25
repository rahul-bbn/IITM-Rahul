#include<stdio.h>
#include<string.h>

int main(){
    char str1[]="silent";
    char str2[]="listen";

    int freq[256]={0};

    if (strlen(str1)!=strlen(str2))
    {
        printf("Not anagram");
        return 0;
    }
    
    for (int i = 0; str1[i]!='\0'; i++)
    {
        freq[str1[i]]++;
        freq[str2[i]]--;
    }
    for (int i = 0; i < 256; i++)
    {
        if (freq[i]!=0)
        {
            printf("Not anagram");
            return 0;
        }
        
    }
    printf("Anagram");    
    return 0;
}