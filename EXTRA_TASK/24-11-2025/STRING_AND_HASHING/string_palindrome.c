#include<stdio.h>
#include<string.h>

int main(){
    char str[]="madam";
    int n=strlen(str);

    int l=0,r=n-1;
    while (l<r)
    {
        if (str[l]!=str[r])
        {
            printf("Not palindrom");
            return 0;
        }
        l++;
        r--;
    }
    printf("Palindrome");
    return 0;
}