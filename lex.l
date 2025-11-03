%{
    
    #include "main.h"

    #include "./Conditionals/jump.h"
    #include "./ExecGen/exec.h"
    #include "./TreeGen/tree.h"
    #include "./symbolTable/symbol.h"
    #include "./typeTable/type.h"

    #include "y.tab.h"

    int yywrap(); 

%}

digit     [0-9]+
letter     [a-zA-Z]


%%

{digit}+        {   
                    yylval.val = atoi(yytext);
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

"main"          {   return MAIN;    }
"return"        {   return RETURN;  }

{letter}({letter}|{digit}|_)*          {   yylval.name = strdup(yytext);
                    return ID;  
                }

[ \t\n]         {   }
[(){},\[\]]     {   return *yytext; }
.               {   }

%%

int yywrap(){
    return 1;
}