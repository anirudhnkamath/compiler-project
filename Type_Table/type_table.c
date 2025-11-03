#include "type_table.h"
#include "../Symbol_Table/global_symbol_table.h"

table_type* table_head = NULL;
table_type* table_tail = NULL;
struct primitive_types* default_types = NULL;
void yyerror(const char *s);
typedef struct Paramstruct p_node;

table_type* create_type_table(char* name, int size) {
    table_type* new_entry = (table_type*) malloc(sizeof(table_type));

    new_entry->name = strdup(name);
    new_entry->size = size;
    new_entry->num_fields = 0;
    new_entry->fields = NULL;
    new_entry->next = NULL;

    if (table_head == NULL) {
        table_head = table_tail = new_entry;
    } else {
        table_tail->next = new_entry;
        table_tail = new_entry;
    }

    return new_entry;
}



void initialize_type_table() {
    table_head = NULL;
    table_tail = NULL;

    table_type* int_type   = create_type_table("int", 1);
    table_type* str_type   = create_type_table("str", 1);
    table_type* bool_type  = create_type_table("bool", 1);
    table_type* void_type  = create_type_table("void", 0);
    table_type* arr_type   = create_type_table("arr", 1);
    table_type* ptr_type   = create_type_table("ptr", 1);
    table_type* func_type  = create_type_table("func", 0);
    table_type* unset_type = create_type_table("unset", 0);
    table_type* int_ptr_type   = create_type_table("int_ptr", 1);
    table_type* str_ptr_type   = create_type_table("str_ptr", 1);

    default_types = (struct primitive_types*) malloc(sizeof(struct primitive_types));

    default_types->int_type     = int_type;
    default_types->str_type     = str_type;
    default_types->bool_type    = bool_type;
    default_types->void_type    = void_type;
    default_types->arr_type     = arr_type;
    default_types->ptr_type     = ptr_type;
    default_types->unset_type   = unset_type;
    default_types->func_type    = func_type;
    default_types->int_ptr_type = int_ptr_type;
    default_types->str_ptr_type = str_ptr_type;
}