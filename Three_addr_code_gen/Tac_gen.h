#ifndef THREE_ADDRESS_CODE_H
#define THREE_ADDRESS_CODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Parse_Tree/parse_tree.h"

void generate_3AC(ast_node* root);
int generate_expr(ast_node* node, char* result);
void generate_stmt(ast_node* node);
int newLabel();
int newTemp();

#endif