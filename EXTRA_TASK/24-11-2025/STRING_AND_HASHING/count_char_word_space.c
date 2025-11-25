#include<stdio.h>

int main(){
    char str[]="Rahul bansal is engineer";
    int character=0,space=0,word=0;

    for (int i = 0; str[i]!='\0'; i++)
    {
        if (str[i]!=' ')
        {
            character++;
        }
        if (str[i]==' ')
        {
            space++;
            word++;
        }
        
        
    }
    printf("character = %d, space = %d and word = %d",character,space,(word+1));

    return 0;
}