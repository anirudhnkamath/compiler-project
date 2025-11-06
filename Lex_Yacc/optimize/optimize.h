#ifndef EXPR_LIST_H
#define EXPR_LIST_H

#include <stdio.h>

typedef struct Expr_List {
    char* operand1;
    char* operator;
    char* operand2;
    char* result;
    struct Expr_List* next;
} expl_node;

extern expl_node* head;

expl_node* createNode(char* op1, char* opr, char* op2, char* res);
expl_node* lookup(char* op1, char* opr, char* op2);
expl_node* insertExpr(char* op1, char* opr, char* op2, char* res);
void removeNodesWithOperand(char* variable);
void displayList(void);

#endif
