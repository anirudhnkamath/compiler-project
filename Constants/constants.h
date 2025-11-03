#ifndef CONSTANTS_H
#define CONSTANTS_H

/* ==========================================================
   📘 constants.h
   Centralized header for all constant enums and macros
   Used throughout the compiler (AST, Symbol Table, etc.)
   ========================================================== */

/* ----------------------------------------------------------
   🌳 Node Types (for Abstract Syntax Tree)
   ---------------------------------------------------------- */
enum ast_node_type {
    NODE_TYPE_CONNECTOR,
    NODE_TYPE_VALUE,
    NODE_TYPE_STRING,
    NODE_TYPE_ID,
    NODE_TYPE_TUPLE,
    NODE_TYPE_DEREF,
    NODE_TYPE_REF,
    NODE_TYPE_WRITE,
    NODE_TYPE_READ,
    NODE_TYPE_ASSIGN,
    NODE_TYPE_PLUS,
    NODE_TYPE_MINUS,
    NODE_TYPE_MULT,
    NODE_TYPE_DIV,
    NODE_TYPE_MOD,
    NODE_TYPE_LT,
    NODE_TYPE_LE,
    NODE_TYPE_GT,
    NODE_TYPE_GE,
    NODE_TYPE_EQ,
    NODE_TYPE_NE,
    NODE_TYPE_IF,
    NODE_TYPE_IF_ELSE,
    NODE_TYPE_WHILE,
    NODE_TYPE_DO_WHILE,
    NODE_TYPE_REPEAT,
    NODE_TYPE_BREAK,
    NODE_TYPE_CONTINUE,
    NODE_TYPE_ARR_ASSIGN,
    NODE_TYPE_ARR_READ,
    NODE_TYPE_ARRAY,
    NODE_TYPE_2D_ARRAY,
    NODE_TYPE_FUNCT_DEFINITION,
    NODE_TYPE_FUNCT_CALL,
    NODE_TYPE_RETURN
};

/* ----------------------------------------------------------
   🔢 Data Types (for Symbol Table / Type Checking)
   ---------------------------------------------------------- */
   
enum data_type {
    DATA_TYPE_INTEGER,
    DATA_TYPE_BOOLEAN,
    DATA_TYPE_VOID,
    DATA_TYPE_STRING,
    DATA_TYPE_ARRAY,
    DATA_TYPE_INTEGER_PTR,
    DATA_TYPE_STRING_PTR
};

enum error{
   E_REG_FULL = -10
};

#endif /* CONSTANTS_H */