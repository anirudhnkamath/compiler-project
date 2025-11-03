#include "parse_tree.h"
void yyerror(const char *s);

ast_node* create_ast_node(int nodetype, char *name, union Constant value) {
    struct ASTNode *node = (struct ASTNode*)malloc(sizeof(struct ASTNode));
    if (!node) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    node->nodetype = nodetype;
    node->name = name ? strdup(name) : NULL;  
    node->value = value;                      

    node->type = NULL;
    node->arglist = NULL;

    node->ptr1 = node->ptr2 = node->ptr3 = NULL;
    
    node->Gentry = NULL;

    return node;
}



ast_node* create_connector_node(ast_node* left, ast_node* right){

    union Constant val;
    ast_node* connector = create_ast_node(NODE_TYPE_CONNECTOR, NULL, val);

    connector->ptr1 = left;
    connector->ptr2 = right;
    connector->type = default_types->void_type;

    return connector;
}

ast_node* create_id_node(ast_node* id){

    gst_node* global_temp = look_up(id->name);
    if(global_temp == NULL){
        yyerror("ID is not neither declared in local symbol table nor declared in global table.\n");
    }

    id->type = global_temp->type;
    id->Gentry = global_temp;

    return id;
}

ast_node* create_read_node(ast_node* id){

    union Constant value;
    ast_node* read_node = create_ast_node(NODE_TYPE_READ, NULL, value);
    read_node->ptr1 = id;
    read_node->type = default_types->void_type;
    return read_node;
}

ast_node* create_write_node(ast_node* expr){

    if(strcmp(expr->type->name, "bool") == 0){
        yyerror("create node accept either string or integer.\n");
    }

    union Constant value;
    ast_node* write_node = create_ast_node(NODE_TYPE_WRITE, NULL, value);
    write_node->ptr1 = expr;
    write_node->type = default_types->void_type;
    return write_node;

}

ast_node* create_assign_node(ast_node* left, ast_node* right){

    if(strcmp(left->type->name, right->type->name) != 0){
        yyerror("Type Mismatch in Assignment\n");
    }

    union Constant value;
    ast_node* assign_node = create_ast_node(NODE_TYPE_ASSIGN, NULL, value);

    assign_node->ptr1 = left;
    assign_node->ptr2 = right;
    assign_node->type = default_types->void_type;
    
    return assign_node;

}

ast_node* create_arithmatic_node(int node_type, ast_node* left, ast_node* right){

    if(strcmp(left->type->name, "int") != 0 || strcmp(right->type->name, "int") != 0){
        yyerror("Type Mismatch in operator node\n");
    }

    union Constant value;
    ast_node* operator_node = create_ast_node(node_type, NULL, value);
    operator_node->ptr1 = left;
    operator_node->ptr2 = right;
    operator_node->type = default_types->int_type;
    return operator_node;
}

ast_node* create_boolean_node(int node_type, ast_node* left, ast_node* right){

    if(strcmp(left->type->name, "int") != 0 || strcmp(right->type->name, "int") != 0){
        yyerror("Type Mismatch in boolean node\n");
    }

    union Constant value;
    ast_node* boolean_node = create_ast_node(node_type, NULL, value);
    boolean_node->ptr1 = left;
    boolean_node->ptr2 = right;
    boolean_node->type = default_types->bool_type;

    return boolean_node;
}

ast_node* assign_type_num_node(ast_node* num){
    num->type = default_types->int_type;
    return num;
}

ast_node* assign_type_str_node(ast_node* str){
    str->type = default_types->str_type;
    return str;
}


ast_node* create_if_else_node(ast_node* condition, ast_node* thenBranch, ast_node* elseBranch){

    if(strcmp(condition->type->name, "bool") != 0){
        yyerror("Condition in IF-ELSE should be Boolean\n");
    }
    union Constant val;
    ast_node* if_else_node = create_ast_node(NODE_TYPE_IF_ELSE, NULL, val);

    if_else_node->ptr1 = condition;
    if_else_node->ptr2 = thenBranch;
    if_else_node->ptr3 = elseBranch;

    if_else_node->type = default_types->void_type;

    return if_else_node;
}

ast_node* create_if_node(ast_node* condition, ast_node* thenBranch){

    if(strcmp(condition->type->name, "bool") != 0){
        yyerror("Condition in IF_NODE should be Boolean\n");
    }
    union Constant val;
    ast_node* if_node = create_ast_node(NODE_TYPE_IF, NULL, val);

    if_node->ptr1 = condition;
    if_node->ptr2 = thenBranch;
    if_node->type = default_types->void_type;

    return if_node;
}

ast_node* create_loop_node(int node_type, ast_node* condition, ast_node* body){

    if(strcmp(condition->type->name, "bool") != 0){
        yyerror("Condition in WHILE should be Boolean\n");
    }

    union Constant val;

    ast_node* loop_node = create_ast_node(node_type, NULL, val);

    loop_node->ptr1 = condition;
    loop_node->ptr2 = body;
    loop_node->type = default_types->void_type;

    return loop_node;
}


ast_node* create_array_node(ast_node* array, ast_node* index){

    gst_node* temp = look_up(array->name);
    if(temp->inner_size == 0){
        yyerror("VARIABLE MISMATCH.\n");
    }

    array->type = temp->type;
    array->Gentry = temp;
    
    union Constant val;
    ast_node* expr_node = create_ast_node(NODE_TYPE_ARRAY, NULL, val);
    expr_node->type = temp->type;
    expr_node->ptr1 = array;
    expr_node->ptr2 = index;
    
    return expr_node;
}


void inorder(ast_node* node) {
    if (node == NULL)
        return;

    switch (node->nodetype) {

        /* =============================
           🚧 Connector Node (Slist)
        ============================== */
        case NODE_TYPE_CONNECTOR:
            inorder(node->ptr1);
            inorder(node->ptr2);
            break;

        /* =============================
           ✏️ Assignment Statement
        ============================== */
        case NODE_TYPE_ASSIGN:
            inorder(node->ptr1);
            printf(" = ");
            inorder(node->ptr2);
            printf("\n");
            break;

        /* =============================
           🧮 Arithmetic / Boolean Operators
        ============================== */
        case NODE_TYPE_PLUS:
        case NODE_TYPE_MINUS:
        case NODE_TYPE_MULT:
        case NODE_TYPE_DIV:
        case NODE_TYPE_MOD:
        case NODE_TYPE_LT:
        case NODE_TYPE_LE:
        case NODE_TYPE_GT:
        case NODE_TYPE_GE:
        case NODE_TYPE_EQ:
        case NODE_TYPE_NE:
            printf("(");
            inorder(node->ptr1);
            switch (node->nodetype) {
                case NODE_TYPE_PLUS:  printf(" + "); break;
                case NODE_TYPE_MINUS: printf(" - "); break;
                case NODE_TYPE_MULT:  printf(" * "); break;
                case NODE_TYPE_DIV:   printf(" / "); break;
                case NODE_TYPE_MOD:   printf(" %% "); break;
                case NODE_TYPE_LT:    printf(" < "); break;
                case NODE_TYPE_LE:    printf(" <= "); break;
                case NODE_TYPE_GT:    printf(" > "); break;
                case NODE_TYPE_GE:    printf(" >= "); break;
                case NODE_TYPE_EQ:    printf(" == "); break;
                case NODE_TYPE_NE:    printf(" != "); break;
            }
            inorder(node->ptr2);
            printf(")");
            break;

        /* =============================
           🔢 Leaf Nodes
        ============================== */
        case NODE_TYPE_ID:
            printf("%s", node->name);
            break;

        case NODE_TYPE_VALUE:
            printf("%d", node->value.iVal);
            break;

        case NODE_TYPE_STRING:
            printf("\"%s\"", node->value.sVal);
            break;

        /* =============================
           📥 Input / Output Statements
        ============================== */
        case NODE_TYPE_READ:
            printf("READ(");
            inorder(node->ptr1);
            printf(");\n");
            break;

        case NODE_TYPE_WRITE:
            printf("WRITE(");
            inorder(node->ptr1);
            printf(");\n");
            break;

        /* =============================
           🧩 Control Structures
        ============================== */
        case NODE_TYPE_IF:
            printf("IF(");
            inorder(node->ptr1);
            printf(")\n{\n");
            inorder(node->ptr2);
            printf("}\n");
            break;

        case NODE_TYPE_IF_ELSE:
            printf("IF(");
            inorder(node->ptr1);
            printf(")\n{\n");
            inorder(node->ptr2);
            printf("}\nELSE\n{\n");
            inorder(node->ptr3);
            printf("}\n");
            break;

        case NODE_TYPE_WHILE:
            printf("WHILE(");
            inorder(node->ptr1);
            printf(")\n{\n");
            inorder(node->ptr2);
            printf("}\n");
            break;

        case NODE_TYPE_DO_WHILE:
            printf("DO {\n");
            inorder(node->ptr2);
            printf("} WHILE(");
            inorder(node->ptr1);
            printf(");\n");
            break;

        case NODE_TYPE_REPEAT:
            printf("REPEAT {\n");
            inorder(node->ptr2);
            printf("} UNTIL(");
            inorder(node->ptr1);
            printf(");\n");
            break;

        /* =============================
           🔁 Array / Pointer Access
        ============================== */
        case NODE_TYPE_ARRAY:
            printf("%s[", node->ptr1->name);
            inorder(node->ptr2);
            printf("]");
            break;

        case NODE_TYPE_DEREF:
            printf("*");
            printf("%s", node->name);
            break;

        case NODE_TYPE_REF:
            printf("&");
            printf("%s", node->name);
            break;

        /* =============================
           📦 Function Calls
        ============================== */
        case NODE_TYPE_FUNCT_CALL:
            printf("%s(...)", node->name);
            break;

        case NODE_TYPE_RETURN:
            printf("return ");
            inorder(node->ptr1);
            printf(";\n");
            break;

        default:
            printf("[Unknown Node Type: %d]\n", node->nodetype);
    }
}