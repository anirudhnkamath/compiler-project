#ifndef TYPE_TABLE_H
#define TYPE_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Paramstruct;
typedef struct Paramstruct p_node;

typedef struct Table_type table_type;
typedef struct Field field;

struct Field {
    char* name;
    int field_index;
    table_type* type;
    field* next;
};

struct Table_type {
    char* name;
    int size;
    int num_fields;
    field* fields;
    table_type* next;
};

struct primitive_types {
    table_type* int_type;
    table_type* ptr_type;
    table_type* arr_type;
    table_type* str_type;
    table_type* void_type;
    table_type* bool_type;
    table_type* unset_type;
    table_type* func_type;
    table_type* int_ptr_type;
    table_type* str_ptr_type;

};

extern struct primitive_types* default_types;
extern table_type* table_head;
extern table_type* table_tail;


table_type* create_type_table(char* name, int size);
void initialize_type_table();

#endif