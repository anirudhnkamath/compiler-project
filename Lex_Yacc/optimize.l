%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lineNum = 0;
char currentLine[512] = "";
int hasConditional = 0;

void processLine();
%}

%option noyywrap

%%

"if("                           { hasConditional = 1; strcpy(currentLine, yytext); return 1; }
"goto"                          { hasConditional = 1; strcpy(currentLine, yytext); return 1; }
"L"[0-9]+":"                    { hasConditional = 1; strcpy(currentLine, yytext); return 1; }

"read"                          { strcpy(currentLine, yytext); return 4; }
"write"                         { strcpy(currentLine, yytext); return 4; }
"t"[0-9]+[ \t]*"="              { strcpy(currentLine, yytext); return 3; }
[a-zA-Z_][a-zA-Z0-9_]*" = "     { strcpy(currentLine, yytext); return 2; }
[ \t]+                          { strcat(currentLine, yytext); }
"t"[0-9]+                       { strcat(currentLine, yytext); }
[a-zA-Z_][a-zA-Z0-9_]*          { strcat(currentLine, yytext); }
[0-9]+                          { strcat(currentLine, yytext); }
"["                             { strcat(currentLine, yytext); }
"]"                             { strcat(currentLine, yytext); }
[+\-*/%]                        { strcat(currentLine, yytext); }
"("                             { strcat(currentLine, yytext); }
")"                             { strcat(currentLine, yytext); }
"<"                             { strcat(currentLine, yytext); }
">"                             { strcat(currentLine, yytext); }
","                             { strcat(currentLine, yytext); }
"="                             { strcat(currentLine, yytext); }
\n                              { lineNum++; currentLine[0] = '\0'; }
.                               { strcat(currentLine, yytext); }

%%