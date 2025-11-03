#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../Symbol_Table/global_symbol_table.h"
#include "./../Type_Table/type_table.h"
#include "./../Constants/constants.h"

typedef struct ASTNode ast_node;

union Constant{
    int iVal;
    char* sVal;
};

struct ASTNode{
  table_type* type;               
  int nodetype;                     
  char *name;                       
  union Constant value;             
  struct ASTNode *arglist;          
  struct ASTNode *ptr1,*ptr2,*ptr3; 
  struct Gsymbol *Gentry;                    
};

struct ASTNode* create_ast_node(int nodetype, char *name, union Constant value);
ast_node* create_connector_node(ast_node* left, ast_node* right);

ast_node* create_id_node(ast_node* id);

ast_node* create_arithmatic_node(int node_type, ast_node* left, ast_node* right);
ast_node* create_boolean_node(int node_type, ast_node* left, ast_node* right);

ast_node* assign_type_num_node(ast_node* num);
ast_node* assign_type_str_node(ast_node* str);

ast_node* create_read_node(ast_node* id);
ast_node* create_write_node(ast_node* expr);
ast_node* create_assign_node(ast_node* left, ast_node* right);

ast_node* create_if_else_node(ast_node* condition, ast_node* thenBranch, ast_node* elseBranch);
ast_node* create_if_node(ast_node* condition, ast_node* thenBranch);

ast_node* create_array_node(ast_node* array, ast_node* index);
ast_node* create_loop_node(int node_type, ast_node* condition, ast_node* body);

void inorder(ast_node* node);

#endif