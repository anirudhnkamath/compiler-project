%{
    
    #include "main.h"

    int yyerror();
    int yylex();

    FILE *output;

    struct GSymbol* GST = NULL;\

%}

%union {
    int val;   
    char* name;
}

%token <name> ID
%token <val> NUM 

%token ADD SUB STAR DIV MOD
%token T_BEGIN T_END READ WRITE ASSGN EOL

%token IF THEN ELSE ENDIF 
%token GE LE EQ GT LT NE 
%token WHILE DO END_WHILE REPEAT UNTILL 
%token DECL ENDDECL 
%token INT

%right ASSGN              
%left OR                  
%left AND                  
%nonassoc EQ NE GT GE LT LE
%left ADD SUB           
%left STAR DIV MOD  

%%

program     :   GDeclBlock Coderegion  
            ;
/* -------------------------------------------------------------------------------------------------- */

GDeclBlock  :   DECL GDeclList ENDDECL          {   }
            |   DECL ENDDECL                    {   }
            ;
GDeclList   :   GDeclList GDecl
            |   GDecl
GDecl       :   Type GidList EOL                {  }
            ;
Type        :   INT                             {  }
            ;
GidList     :   GidList ',' Gid
            |   Gid
            ;
Gid         :   ID                              {      }
            |   ID '[' NUM ']'                  {      }
            ;

/* ---------------------------------------------------------------------------------- */

Coderegion  :   T_BEGIN Slist T_END                 {   }
            ;
Slist       :   Slist Stmt                          {   }
            |   Stmt                                {   }
            ;
Stmt        :   InputStmt                           {   }
            |   OutputStmt                          {   }
            |   AsgnStmt                            {   }
            |   IfStmt                              {   }
            |   WhileStmt                           {   }
            |   RptUntlStmt                         {   }
            |   DoWhileStmt                         {   }
            ;
InputStmt   :   READ'('IDENTIFIERS')' EOL           {   }
            ;
OutputStmt  :   WRITE'(' expr ')' EOL               {   }
            ;
AsgnStmt    :   IDENTIFIERS ASSGN expr EOL          {   }
            ;

IfStmt      :   IF '(' expr ')' THEN Slist ELSE Slist ENDIF EOL  {    }
            |   IF '(' expr ')' THEN Slist ENDIF EOL             {    }
            ;
WhileStmt   :   WHILE '(' expr ')' DO Slist END_WHILE EOL        {     }
            ;
RptUntlStmt :   REPEAT '{' Slist '}' UNTILL '(' expr ')' EOL     {     }
            ;
DoWhileStmt :   DO '{' Slist '}'  WHILE '(' expr ')' EOL         {     }
            ;

expr        :   expr ADD expr                       {    }
            |   expr SUB expr                       {    }
            |   expr STAR expr                      {    }
            |   expr DIV expr                       {    }
            |   expr MOD expr                       {    }
            |   expr EQ expr                        {    }
            |   expr NE expr                        {    }
            |   expr GT expr                        {    }
            |   expr GE expr                        {    }
            |   expr LT expr                        {    }
            |   expr LE expr                        {    }
            |   expr OR expr                        {    }
            |   expr AND expr                       {    }
            |   '(' expr ')'                        {    }
            |   NUM                                 {    }
            |   IDENTIFIERS                         {    }
            ;

IDENTIFIERS :   ID                                  {    }
            |   ID '[' expr ']'                     {    }
            ;
        
%%

extern FILE* yyin;

int yyerror(const char* s){
    printf("Error: %s\n", s);
    return 1;
}

int main(int argc, char **argv){

    if(argc > 1){
        yyin = fopen(argv[1], "r");
        if(!yyin){
            printf("Error opening file\n");
            return 1;
        }
    }
    output = fopen("output.xsm", "w");
    printGST();
    fclose(output);
    return 0;
}