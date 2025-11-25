#include<stdio.h>
#include<string.h>

void reverse(char *str,int start,int end){
    while (start<end)
    {
        char temp=str[start];
        str[start]=str[end];
        str[end]=temp;

        start++;
        end--;
    }
    
}

int main(){
    char str[50];

    printf("Enter any string:");
    scanf("%[^\n]s",str);

    int start=0,end=strlen(str);
    // reverse(str,0,end-1);

    for (int i = 0; i <=end; i++)
    {
        if (str[i]==' '||str[i]=='\0')
        {
            reverse(str,start,i-1);
            start=i+1;
        }
        
    }
    printf("reverse string = %s",str);
    return 0;
}