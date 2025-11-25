#include<stdio.h>
#include<string.h>

void reverse(char *str,int l,int r){
    while (l<r)
    {
        int temp=str[l];
        str[l]=str[r];
        str[r]=temp;

        l++;
        r--;
    }
    
}

int main(){
    char str[]="Rahul";
    int n=strlen(str);
    int l=0,r=n-1;

    printf("Before reverse string is: %s",str);

    reverse(str,l,r);
    printf("\nAfter reverse: %s",str);
    return 0;
}