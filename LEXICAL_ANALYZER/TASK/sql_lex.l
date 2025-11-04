%{
#include "sql_lex.tab.h"
#include<string.h>
#include<stdlib.h>
%}

%%

SELECT    {return SELECT_TOKEN;}
FROM      {return FROM_TOKEN;}
WHERE     {return WHERE_TOKEN;}

[0-9]+    {yylval.num=atoi(yytext); return NUMBER;}
[a-zA-Z_][a-zA-Z0-9_]*    {yylval.str=strdup(yytext); return IDENTIFIER;}

"="       {return EQ;}
";"       {return SEMICOLON;}
[ \t\n]+   {/*ignore white spaces*/}
.         {return yytext[0];}

%%

int yywrap(void) {return 1;}