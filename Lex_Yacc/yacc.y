
%{
    #include "./../Parse_Tree/parse_tree.h"
    #include "./../Symbol_Table/global_symbol_table.h"
    #include "./../Three_addr_code_gen/Tac_gen.h"
    void yyerror(const char* error);
    int yylex(void);
%}

%union {
    ast_node* AST_node;
}

%token <AST_node> ID NUM


%token ADD SUB STAR DIV MOD
%token T_BEGIN T_END READ WRITE ASSGN EOL

%token IF THEN ELSE ENDIF 
%token GE LE EQ GT LT NE 
%token WHILE DO END_WHILE REPEAT UNTILL 
%token DECL ENDDECL 
%token INT

%type <AST_node> expr IDENTIFIERS WhileStmt RptUntlStmt DoWhileStmt IfStmt AsgnStmt OutputStmt InputStmt Stmt Slist

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

GDeclBlock  :   DECL GDeclList ENDDECL          { }
            |   DECL ENDDECL                    {   }
            ;
GDeclList   :   GDeclList GDecl
            |   GDecl
            ;

GDecl       :   INT GidList EOL                {  }
            ;

GidList     :   GidList ',' Gid
            |   Gid
            ;

Gid         :   ID                              {  gst_install($1->name, default_types->int_type, 1, 0, NULL); }
            |   ID '[' NUM ']'                  {  gst_install($1->name, default_types->int_type, 1, $3->value.iVal, NULL); }
            ;

/* ---------------------------------------------------------------------------------- */

Coderegion  :   T_BEGIN Slist T_END                 { generate_3AC($2); }
            ;
Slist       :   Slist Stmt                          { $$ = create_connector_node($1, $2); }
            |   Stmt                                { $$ = $1; }
            ;
Stmt        :   InputStmt                           { $$ = $1; }
            |   OutputStmt                          { $$ = $1; }
            |   AsgnStmt                            { $$ = $1; }
            |   IfStmt                              { $$ = $1; }
            |   WhileStmt                           { $$ = $1; }
            |   RptUntlStmt                         { $$ = $1; }
            |   DoWhileStmt                         { $$ = $1; }
            ;
InputStmt   :   READ'('IDENTIFIERS')' EOL           {  $$ = create_read_node($3); }
            ;
OutputStmt  :   WRITE'(' expr ')' EOL               {  $$ = create_write_node($3); }
            ;
AsgnStmt    :   IDENTIFIERS ASSGN expr EOL          { $$ = create_assign_node($1, $3); }
            ;

IfStmt      :   IF '(' expr ')' THEN Slist ELSE Slist ENDIF EOL  {  $$ = create_if_else_node($3, $6, $8);  }
            |   IF '(' expr ')' THEN Slist ENDIF EOL             {  $$ = create_if_node($3, $6); }
            ;
WhileStmt   :   WHILE '(' expr ')' DO Slist END_WHILE EOL        {  $$ = create_loop_node(NODE_TYPE_WHILE, $3, $6); }
            ;
RptUntlStmt :   REPEAT '{' Slist '}' UNTILL '(' expr ')' EOL     {  $$ = create_loop_node(NODE_TYPE_REPEAT, $7, $3); }
            ;
DoWhileStmt :   DO '{' Slist '}'  WHILE '(' expr ')' EOL         {  $$ = create_loop_node(NODE_TYPE_DO_WHILE, $7, $3); }
            ;

expr        :   expr ADD expr                       { $$ = create_arithmatic_node(NODE_TYPE_PLUS, $1, $3);  }
            |   expr SUB expr                       { $$ = create_arithmatic_node(NODE_TYPE_MINUS, $1, $3);  }
            |   expr STAR expr                      { $$ = create_arithmatic_node(NODE_TYPE_MULT, $1, $3);  }
            |   expr DIV expr                       { $$ = create_arithmatic_node(NODE_TYPE_DIV, $1, $3);  }
            |   expr MOD expr                       { $$ = create_arithmatic_node(NODE_TYPE_MOD, $1, $3);  }
            |   expr EQ expr                        { $$ = create_boolean_node(NODE_TYPE_EQ, $1, $3);  }
            |   expr NE expr                        { $$ = create_boolean_node(NODE_TYPE_NE, $1, $3);  }
            |   expr GT expr                        { $$ = create_boolean_node(NODE_TYPE_GT, $1, $3);  }
            |   expr GE expr                        { $$ = create_boolean_node(NODE_TYPE_GE, $1, $3);  }
            |   expr LT expr                        { $$ = create_boolean_node(NODE_TYPE_LT, $1, $3);  }
            |   expr LE expr                        { $$ = create_boolean_node(NODE_TYPE_LE, $1, $3);  }
            |   '(' expr ')'                        { $$ = $2;  }
            |   NUM                                 { $$ = assign_type_num_node($1); }
            |   IDENTIFIERS                         { $$ = $1;  }
            ;

IDENTIFIERS :   ID                                  {  $$ = create_id_node($1);  }
            |   ID '[' expr ']'                     {  $$ = create_array_node($1, $3);  }
            ;
        
%%

extern FILE* yyin;

void yyerror(const char* s){
    printf("Error: %s", s);
    exit(1);
    return ;
}

int main(int argc, char **argv){

    initialize_type_table();

    if(argc > 1){
        yyin = fopen(argv[1], "r");
        if(!yyin){
            printf("Error opening file\n");
            return 1;
        }
    }
    yyparse();

    return 0;
}