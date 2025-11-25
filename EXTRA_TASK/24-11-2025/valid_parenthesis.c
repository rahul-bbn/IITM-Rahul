#include<stdio.h>
#include<string.h>
#include<stdbool.h>

int isMatching(char open, char close){
    return (open=='(' && close==')' || open=='{' && close=='}' || open=='[' || close==']');
}

bool isValid(char *str){
    int n=strlen(str);
    char stack[n];
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
    char str[]=" [a{{(ba)}]";

    if (isValid(str))
    {
        printf("Valid parenthesis");
    }else{
        printf("InValid parenthesis");
    }
    
    return 0;
}