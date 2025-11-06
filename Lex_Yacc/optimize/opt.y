%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include "./opt.h"

    int yylex();
    int yyerror(const char* msg);
    int checkForConditional(FILE* file);
    extern FILE* yyin;

    FILE* inputFile, *outputFile;
%}

%token ASSGN
%token READ WRITE
%token EOL
%token ID
%token OP

%type <strVal> ID OP

%start start

%union {
    char* strVal;
}

%%

start   :   stmtList    {printf("Successful\n"); exit(1);}
        ;

stmtList:   stmtList stmt
        |   stmt
        ;

stmt    :   writeStmt
        |   readStmt
        |   assgStmt
        ;

writeStmt   :   WRITE '(' ID ')' EOL                {   fprintf(outputFile, "write(%s)\n", $3); }
            ;

readStmt    :   READ '(' ID ')' EOL                 { 
                                                        fprintf(outputFile, "read(%s)\n", $3); 
                                                        removeNodesWithOperand($3);
                                                    }
            |   READ '(' ID '[' ID ']' ')' EOL      {   fprintf(outputFile, "read(%s[%s])\n", $3, $5); }
            ;

assgStmt    :   ID ASSGN ID EOL                     {
                                                        fprintf(outputFile, "%s = %s\n", $1, $3);
                                                        removeNodesWithOperand($1);
                                                    }
            |   ID ASSGN ID OP ID EOL               {
                                                        expl_node* found = lookup($3, $4, $5);
                                                        if(!found) {
                                                            fprintf(outputFile, "%s = %s %s %s\n", $1, $3, $4, $5);
                                                            insertExpr($3, $4, $5, $1);
                                                        }
                                                        else {
                                                            fprintf(outputFile, "%s = %s\n", $1, found->result);
                                                        }
                                                        removeNodesWithOperand($1);
                                                    }
            |   ID ASSGN ID '[' ID ']' EOL          {
                                                        fprintf(outputFile, "%s = %s[%s]\n", $1, $3, $5);
                                                        removeNodesWithOperand($1);
                                                    }
            |   ID '[' ID ']' ASSGN ID EOL          {
                                                        fprintf(outputFile, "%s[%s] = %s\n", $1, $3, $6);
                                                    }
            ;

%%

int yyerror(const char* msg) {
    printf("Error : %s\n", msg);
    exit(1);
    return 1;
}

int checkForConditional(FILE* file) {
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char* ptr = line;
        if (*ptr == 'L') {
            ptr++;
            int hasDigit = 0;
            while (isdigit(*ptr)) {
                hasDigit = 1;
                ptr++;
            }
            if (hasDigit && *ptr == ':') {
                return 1;
            }
        }
    }

    return 0;
}

int main() {
    inputFile = fopen("targetFile.txt", "r");
    outputFile = fopen("opt.txt", "w");

    if(checkForConditional(inputFile) == 1) {
        printf("Conditional detected, no change made\n");
        return 0;
    }

    rewind(inputFile);
    yyin = inputFile;
    yyparse();

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}