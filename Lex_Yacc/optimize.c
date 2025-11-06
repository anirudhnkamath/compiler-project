#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// External variables from lexer
extern int lineNum;
extern char currentLine[512];
extern int hasConditional;

// Linked list node for storing expressions
typedef struct ExprNode {
    char lhs[64];
    char rhs[256];
    struct ExprNode* next;
} ExprNode;

ExprNode* exprList = NULL;

// Function prototypes
extern int yylex();
extern FILE* yyin;
void addOrUpdateExpr(const char* lhs, const char* rhs);
char* findExpr(const char* rhs);
void invalidateVar(const char* varName);
void freeExprList();
void optimizeLine(char* line);

int main(int argc, char** argv) {
    FILE* inputFile;
    FILE* outputFile;

    if (argc < 3) {
        printf("Usage: %s <input_tac_file> <output_optimized_file>\n", argv[0]);
        return 1;
    }

    inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        printf("Error: Cannot open input file %s\n", argv[1]);
        return 1;
    }

    outputFile = fopen(argv[2], "w");
    if (!outputFile) {
        printf("Error: Cannot create output file %s\n", argv[2]);
        fclose(inputFile);
        return 1;
    }

    yyin = inputFile;
    
    // First pass: Check for conditionals
    while (yylex() != 0);
    
    if (hasConditional) {
        printf("Conditional detected, no changes made\n");
        rewind(inputFile);
        char line[512];
        while (fgets(line, sizeof(line), inputFile)) {
            fprintf(outputFile, "%s", line);
        }
    } else {
        printf("CSE started\n");
        fclose(inputFile);
        inputFile = fopen(argv[1], "r");
        
        char line[512];
        while (fgets(line, sizeof(line), inputFile)) {
            optimizeLine(line);
            fprintf(outputFile, "%s", line);
        }
        printf("Process complete\n");
    }

    fclose(inputFile);
    fclose(outputFile);
    freeExprList();
    return 0;
}

void optimizeLine(char* line) {

    // Handle read statement - invalidate the variable
    if (strstr(line, "read(") != NULL) {
        char* start = strstr(line, "read(") + 5;
        char varName[64];
        int i = 0;
        while (start[i] != ')' && start[i] != '\0' && start[i] != ' ') {
            varName[i] = start[i];
            i++;
        }
        varName[i] = '\0';
        invalidateVar(varName);
        return;
    }
    
    // Parse assignment: LHS = RHS
    char* eqPos = strchr(line, '=');
    if (eqPos == NULL) return;
    
    char lhs[64], rhs[256];
    
    // Extract LHS
    char* lhsStart = line;
    while (*lhsStart == ' ' || *lhsStart == '\t') lhsStart++;
    char* lhsEnd = eqPos - 1;
    while (*lhsEnd == ' ' || *lhsEnd == '\t') lhsEnd--;
    int lhsLen = lhsEnd - lhsStart + 1;
    strncpy(lhs, lhsStart, lhsLen);
    lhs[lhsLen] = '\0';
    
    // Extract RHS
    char* rhsStart = eqPos + 1;
    while (*rhsStart == ' ' || *rhsStart == '\t') rhsStart++;
    char* rhsEnd = rhsStart + strlen(rhsStart) - 1;
    while (*rhsEnd == '\n' || *rhsEnd == ' ' || *rhsEnd == '\t') rhsEnd--;
    int rhsLen = rhsEnd - rhsStart + 1;
    strncpy(rhs, rhsStart, rhsLen);
    rhs[rhsLen] = '\0';
    
    // Check if RHS has operators (is it a computation?)
    int hasOp = 0;
    for (int i = 0; rhs[i]; i++) {
        if (rhs[i] == '+' || rhs[i] == '-' || rhs[i] == '*' || 
            rhs[i] == '/' || rhs[i] == '%') {
            hasOp = 1;
            break;
        }
    }
    
    if (hasOp) {
        // Check if this expression already exists
        char* existingVar = findExpr(rhs);
        if (existingVar != NULL) {
            sprintf(line, "%s = %s\n", lhs, existingVar);
            printf("Optimized: %s = %s (CSE applied)\n", lhs, existingVar);
        } else {
            addOrUpdateExpr(lhs, rhs);
        }
    }
    
    // Always invalidate LHS variable from expression list
    invalidateVar(lhs);
}

void addOrUpdateExpr(const char* lhs, const char* rhs) {
    ExprNode* newNode = (ExprNode*)malloc(sizeof(ExprNode));
    strcpy(newNode->lhs, lhs);
    strcpy(newNode->rhs, rhs);
    newNode->next = exprList;
    exprList = newNode;
}

char* findExpr(const char* rhs) {
    ExprNode* current = exprList;
    while (current != NULL) {
        if (strcmp(current->rhs, rhs) == 0) {
            return current->lhs;
        }
        current = current->next;
    }
    return NULL;
}

void invalidateVar(const char* varName) {
    ExprNode* prev = NULL;
    ExprNode* current = exprList;
    
    while (current != NULL) {
        int usesVar = 0;
        
        // Check if varName appears in RHS as a complete identifier
        const char* pos = current->rhs;
        int len = strlen(varName);
        while ((pos = strstr(pos, varName)) != NULL) {
            int before_ok = (pos == current->rhs) || 
                           (!((pos[-1] >= 'a' && pos[-1] <= 'z') || 
                              (pos[-1] >= 'A' && pos[-1] <= 'Z') || 
                              (pos[-1] >= '0' && pos[-1] <= '9') || 
                              pos[-1] == '_'));
            int after_ok = (pos[len] == '\0') || 
                          (!((pos[len] >= 'a' && pos[len] <= 'z') || 
                             (pos[len] >= 'A' && pos[len] <= 'Z') || 
                             (pos[len] >= '0' && pos[len] <= '9') || 
                             pos[len] == '_'));
            
            if (before_ok && after_ok) {
                usesVar = 1;
                break;
            }
            pos += len;
        }
        
        if (usesVar) {
            printf("  Invalidating: %s = %s\n", current->lhs, current->rhs);
            ExprNode* toDelete = current;
            if (prev == NULL) {
                exprList = current->next;
                current = exprList;
            } else {
                prev->next = current->next;
                current = current->next;
            }
            free(toDelete);
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void freeExprList() {
    ExprNode* current = exprList;
    while (current != NULL) {
        ExprNode* next = current->next;
        free(current);
        current = next;
    }
}