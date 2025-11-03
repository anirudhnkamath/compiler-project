%{

    #include "y.tab.h"
    #include "./../Parse_Tree/parse_tree.h"

    int yywrap(); 

%}

digit     [0-9]+
letter     [a-zA-Z]


%%

{digit}+        {
                    union Constant val;
                    val.iVal = atoi(yytext);
                    yylval.AST_node = create_ast_node(NODE_TYPE_VALUE, NULL, val);
                    return NUM;
                }

"+"             {   return ADD;    }
"-"             {   return SUB;    }
"%"             {   return MOD;    }
"*"             {   return STAR;   }
"/"             {   return DIV;    }
"="             {   return ASSGN;   }

";"             {   return EOL;  }
"begin"         {   return T_BEGIN;   }
"end"           {   return T_END; }

"read"          {   return READ;    }
"write"         {   return WRITE;   }

"if"            {   return IF;  }
"then"          {   return THEN;    }
"else"          {   return ELSE;    }
"endif"         {   return ENDIF;   }

"while"         {   return WHILE;   }
"do"            {   return DO;  }
"endwhile"      {   return END_WHILE;    }

"repeat"        {   return REPEAT;  }
"untill"        {   return UNTILL;  }

"<="            {   return LE;  }
">="            {   return GE;  }
"=="            {   return EQ;  }
"!="            {   return NE;  }
"<"             {   return LT;  }
">"             {   return GT;  }
"||"            {   return OR;  }
"&&"            {   return AND; }

"decl"          {   return DECL;    }
"enddecl"       {   return ENDDECL; }
"int"           {   return INT; }


{letter}({letter}|{digit}|_)*          {
                                            union Constant val;
                                            yylval.AST_node = create_ast_node(NODE_TYPE_ID, strdup(yytext), val);
                                            return ID;
                                        }

[ \t\n]         {   }
[(){},\[\]]     {   return *yytext; }
.               {   }

%%

int yywrap(){
    return 1;
}