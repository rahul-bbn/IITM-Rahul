#include<stdio.h>
#include<ctype.h>

int main(){
    char str[]="Rahul bansal";
    int vowel=0,cons=0;

    for (int i = 0; str[i] !='\0'; i++)
    {
        char ch=tolower(str[i]);

        if (isalpha(ch))
        {
            if (ch=='a' || ch=='e' || ch=='i' || ch=='o' || ch=='u')
            {
                vowel++;
            }else{
                cons++;
            }
            
        }
        
    }
    printf("Total vowels= %d \nand consonant= %d",vowel,cons);

    return 0;
    
}