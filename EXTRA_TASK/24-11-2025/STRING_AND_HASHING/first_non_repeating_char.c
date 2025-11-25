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

    for (int i = 0; i < str[i]; i++)
    {
        if (freq[str[i]]==1)
        {
            printf("%c",str[i]);
            return 0;
        }
    }
    printf("No Non repeating character");
    return 0;
}