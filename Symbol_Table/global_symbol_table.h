#ifndef GLOBAL_SYMBOL_TABLE_H
#define GLOBAL_SYMBOL_TABLE_H
#include "./../Type_Table/type_table.h"

typedef struct Gsymbol gst_node;
typedef struct Paramstruct p_node;

struct Gsymbol{

    char* name;
    table_type* type;

    int out_size;
    int inner_size;

    int binding;

    int flag_label;

    p_node* param_list;
    gst_node* next;

};

struct Paramstruct{
    char* name;
    table_type* type;
    p_node* next;
};

gst_node* create_gst_node(char* name, table_type* type, int inner_size, int outer_size, p_node* param_list, gst_node* next);
void gst_install(char *name, table_type* type, int outer_size, int inner_size, p_node* param_list);
gst_node* look_up(char* name);
void print_gstable();

#endif
