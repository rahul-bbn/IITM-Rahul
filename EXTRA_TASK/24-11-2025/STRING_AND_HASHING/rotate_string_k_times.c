#include<stdio.h>
#include<string.h>

void reverse(char *str,int left,int right){
    while (left<right)
    {
        char temp=str[left];
        str[left]=str[right];
        str[right]=temp;

        left++;
        right--;
    }
    
}

int main(){
    char str[100];
    int k;

    printf("Enter string:");
    scanf("%s",str);

    printf("Enter k:");
    scanf("%d",&k);

    int n=strlen(str);

    k%=n;

    reverse(str,0,k-1);
    reverse(str,k+1,n-1);
    reverse(str,0,n-1);

    printf("%s",str);

    return 0;
}