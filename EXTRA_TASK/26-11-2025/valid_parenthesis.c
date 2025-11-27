#include<stdio.h>
#include<stdbool.h>
#include<string.h>

int isMatching(char open,char close){
    return (open=='(' && close==')' ||
            open=='{' && close=='}' ||
            open=='[' && close==']');
}

bool isValid(char *str){
    char stack[100];
    int top=-1;
    for (int i = 0; str[i]!='\0'; i++)
    {
        char ch=str[i];
        if (ch=='(' || ch=='{' || ch=='[')
        {
            stack[++top]=ch;
        }else if(ch==')' || ch=='}' || ch==']'){
            if (top==-1)
            {
                return false;
            }
            char last=stack[top--];
            if (!isMatching(last,ch))
            {
                return false;
            }
            
        }
        
    }
    return top==-1;
    
}

int main(){
    char str[]="((a{b[(b)]}))";

    if (isValid(str))
    {
       printf("valid parentheses");
    }else{
        printf("Invalid Parentheses");
    }

    
    return 0;
}