%{
#include "lex_analyzer.tab.h"
%}
 
%%
 
[0-9]+       { yylval.num = atoi(yytext); return NUMBER; }
[ \t]+       {/* ignore spaces */}
"("          { return '('; }
")"          { return ')'; }
"+"          { return '+'; }
"-"          { return '-'; }
"*"          { return '*'; }
"/"          { return '/'; }
\n           { return '\n'; }
 
.            { return yytext[0]; }  // any other char
 
%%
 
int yywrap(void) { return 1; }