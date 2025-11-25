#include<stdio.h>
#include<string.h>

void reverseStr(char *str){
    int n=strlen(str);
    int stack[n];
    int top=-1;

    for (int i = 0; str[i]!='\0'; i++)
    {
        stack[++top]=str[i];
    }
    for (int i = top; i >=0; i--)
    {
        printf("%c",stack[i]);
    }
}

int main(){
    char str[]="Rahul Bansal";

    reverseStr(str);
    return 0;
}