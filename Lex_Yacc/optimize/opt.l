%{
    #include "y.tab.h"
    int yywrap();
%}

digit       [0-9]
letter      [a-zA-Z]

%%

[\+\-\%\*\/]    {   
                    yylval.strVal = strdup(yytext);
                    return OP;
                }

"="             {   return ASSGN;  }

"\n"             {   return EOL; }

"read"          {   return READ;   }
"write"         {   return WRITE;  }

[\[\]()]     {   return *yytext; }

({letter}|{digit})+     {   
                                yylval.strVal = strdup(yytext);
                                return ID;
                        }

[ \t\n]         {  }
.               {  }

%%

int yywrap(){
    return 1;
}