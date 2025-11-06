#include "Tac_gen.h"
#include <stdio.h>

static int tempCount = 0;
static int labelCount = 0;

FILE *tacFile;

int newTemp() {
    return ++tempCount;
}

int newLabel() {
    return ++labelCount;
}

const char* getOp(int type) {
    switch(type) {
        case NODE_TYPE_PLUS:  return "+";
        case NODE_TYPE_MINUS: return "-";
        case NODE_TYPE_MULT:  return "*";
        case NODE_TYPE_DIV:   return "/";
        case NODE_TYPE_MOD:   return "%";

        case NODE_TYPE_LT: return "<";
        case NODE_TYPE_LE: return "<=";
        case NODE_TYPE_GT: return ">";
        case NODE_TYPE_GE: return ">=";
        case NODE_TYPE_EQ: return "==";
        case NODE_TYPE_NE: return "!=";

        default: return "?";
    }
}

int generate_expr(ast_node* node, char* result) {
    if (!node) return -1;

    switch (node->nodetype) {

        case NODE_TYPE_VALUE:
            sprintf(result, "%d", node->value.iVal);
            return -1;

        case NODE_TYPE_ID:
            sprintf(result, "%s", node->name);
            return -1;

        case NODE_TYPE_ARRAY: {
            char index[32];
            generate_expr(node->ptr2, index);

            int tOffset = newTemp();
            fprintf(tacFile, "t%d = %s * 4\n", tOffset, index);

            int t = newTemp();
            sprintf(result, "t%d", t);
            fprintf(tacFile, "t%d = %s[t%d]\n", t, node->ptr1->name, tOffset);

            return t;
        }

        case NODE_TYPE_PLUS:
        case NODE_TYPE_MINUS:
        case NODE_TYPE_MULT:
        case NODE_TYPE_DIV:
        case NODE_TYPE_MOD: {
            char left[32], right[32];
            generate_expr(node->ptr1, left);
            generate_expr(node->ptr2, right);

            int t = newTemp();
            sprintf(result, "t%d", t);

            char op;
            switch (node->nodetype) {
                case NODE_TYPE_PLUS:  op = '+'; break;
                case NODE_TYPE_MINUS: op = '-'; break;
                case NODE_TYPE_MULT:  op = '*'; break;
                case NODE_TYPE_DIV:   op = '/'; break;
                case NODE_TYPE_MOD:   op = '%'; break;
                default: op = '?';
            }

            fprintf(tacFile, "%s = %s %c %s\n", result, left, op, right);
            return t;
        }

        default:
            sprintf(result, "?");
            return -1;
    }
}

void generate_stmt(ast_node* node) {
    if (!node) return;

    switch (node->nodetype) {

        case NODE_TYPE_CONNECTOR:
            generate_stmt(node->ptr1);
            generate_stmt(node->ptr2);
            break;

        case NODE_TYPE_ASSIGN: {
            char left[32], right[32];
            generate_expr(node->ptr2, right);

            if(node->ptr1->nodetype == NODE_TYPE_ARRAY) {
                char index[32];
                generate_expr(node->ptr1->ptr2, index);

                int temp = newTemp();
                fprintf(tacFile, "t%d = %s * 4\n", temp, index);
                sprintf(left, "%s[t%d]", node->ptr1->ptr1->name, temp);
            }
            else {
                generate_expr(node->ptr1, left);
            }

            fprintf(tacFile, "%s = %s\n", left, right);
            break;
        }

        case NODE_TYPE_READ: {
            char left[32];
            if(node->ptr1->nodetype == NODE_TYPE_ARRAY) {
                char index[32];
                generate_expr(node->ptr1->ptr2, index);

                int temp = newTemp();
                fprintf(tacFile, "t%d = %s * 4\n", temp, index);
                sprintf(left, "%s[t%d]", node->ptr1->ptr1->name, temp);
            }
            else {
                generate_expr(node->ptr1, left);
            }

            fprintf(tacFile, "read(%s)\n", left);
            break;
        }

        case NODE_TYPE_WRITE: {
            char expr[32];
            generate_expr(node->ptr1, expr);
            fprintf(tacFile, "write(%s)\n", expr);
            break;
        }

        case NODE_TYPE_IF_ELSE: {
            char condLeft[32], condRight[32];
            generate_expr(node->ptr1->ptr1, condLeft);
            generate_expr(node->ptr1->ptr2, condRight);

            int L1 = newLabel();
            int L2 = newLabel();
            int L3 = newLabel();

            fprintf(tacFile, "if (%s %s %s) goto L%d\n", condLeft, getOp(node->ptr1->nodetype), condRight, L1);
            fprintf(tacFile, "goto L%d\n", L2);

            fprintf(tacFile, "L%d:\n", L1);
            generate_stmt(node->ptr2);
            fprintf(tacFile, "goto L%d\n", L3);

            fprintf(tacFile, "L%d:\n", L2);
            generate_stmt(node->ptr3);

            fprintf(tacFile, "L%d:\n", L3);
            break;
        }

        case NODE_TYPE_IF: {
            char condLeft[32], condRight[32];
            generate_expr(node->ptr1->ptr1, condLeft);
            generate_expr(node->ptr1->ptr2, condRight);

            int L1 = newLabel();
            int L2 = newLabel();

            fprintf(tacFile, "if (%s %s %s) goto L%d\n", condLeft, getOp(node->ptr1->nodetype), condRight, L1);
            fprintf(tacFile, "goto L%d\n", L2);

            fprintf(tacFile, "L%d:\n", L1);
            generate_stmt(node->ptr2);

            fprintf(tacFile, "L%d:\n", L2);
            break;
        }

        case NODE_TYPE_WHILE: {
            int L1 = newLabel();
            int L2 = newLabel();
            int L3 = newLabel();

            fprintf(tacFile, "L%d:\n", L1);

            char condLeft[32], condRight[32];
            generate_expr(node->ptr1->ptr1, condLeft);
            generate_expr(node->ptr1->ptr2, condRight);

            fprintf(tacFile, "if (%s %s %s) goto L%d\n", condLeft, getOp(node->ptr1->nodetype), condRight, L2);
            fprintf(tacFile, "goto L%d\n", L3);

            fprintf(tacFile, "L%d:\n", L2);
            generate_stmt(node->ptr2);
            fprintf(tacFile, "goto L%d\n", L1);

            fprintf(tacFile, "L%d:\n", L3);
            break;
        }

        case NODE_TYPE_REPEAT: {
            int Lloop = newLabel();
            int Lexit = newLabel();

            fprintf(tacFile, "L%d:\n", Lloop);

            generate_stmt(node->ptr2);

            char a[32], b[32];
            generate_expr(node->ptr1->ptr1, a);
            generate_expr(node->ptr1->ptr2, b);

            fprintf(tacFile, "if (%s %s %s) goto L%d\n", a, getOp(node->ptr1->nodetype), b, Lexit);
            fprintf(tacFile, "goto L%d\n", Lloop);
            fprintf(tacFile, "L%d:\n", Lexit);

            break;
        }

        case NODE_TYPE_DO_WHILE: {
            int L1 = newLabel();
            fprintf(tacFile, "L%d:\n", L1);
            generate_stmt(node->ptr2);

            char condLeft[32], condRight[32];
            generate_expr(node->ptr1->ptr1, condLeft);
            generate_expr(node->ptr1->ptr2, condRight);

            fprintf(tacFile, "if (%s %s %s) goto L%d\n", condLeft, getOp(node->ptr1->nodetype), condRight, L1);
            break;
        }

        default:
            break;
    }
}

void generate_3AC(ast_node* root) {
    tacFile = fopen("targetFile.txt", "w");
    if (!tacFile) {
        printf("Error: cannot open targetFile.txt\n");
        return;
    }

    generate_stmt(root);

    fclose(tacFile);
}
