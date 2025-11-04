%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int yylex();
void yyerror(const char *s);
%}

%union{
   int num;
   char *str;
}

%token SELECT_TOKEN FROM_TOKEN WHERE_TOKEN
%token EQ SEMICOLON
%token <str> IDENTIFIER
%token <num> NUMBER

%start statements

%%
statements:
       /*empty statement*/
       | statements statement

statement:
       SELECT_TOKEN IDENTIFIER FROM_TOKEN IDENTIFIER WHERE_TOKEN IDENTIFIER EQ NUMBER SEMICOLON
       {
        printf("\nParser SQL Successfully!\n");
        printf("Column: %s\n",$2);
        printf("Table: %s\n",$4);
        printf("where : %s = %d \n",$6,$8);
       }
;

%%

void yyerror(const char *s){
    printf("Syntax Error: %s\n", s);
}

int main(){
    printf("Enter SQL Query:\n");
    yyparse();
    return 0;
}
