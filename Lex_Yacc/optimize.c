#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variables for lexer
int lineNum = 0;
char currentLine[512] = "";
int hasConditional = 0;

// Linked list node for storing expressions
typedef struct ExprNode {
    char lhs[64];        // Left-hand side (result variable)
    char rhs[256];       // Right-hand side (expression)
    struct ExprNode* next;
} ExprNode;

ExprNode* exprList = NULL;

// Function prototypes
extern int yylex();
extern FILE* yyin;
void processLine();
void addExpression(const char* lhs, const char* rhs);
char* findExpression(const char* rhs);
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
        
        // Copy file without optimization
        rewind(inputFile);
        char line[512];
        while (fgets(line, sizeof(line), inputFile)) {
            fprintf(outputFile, "%s", line);
        }

    } else {
        printf("CSE started\n");
        
        // Reopen file for second pass
        fclose(inputFile);
        inputFile = fopen(argv[1], "r");
        yyin = inputFile;
        lineNum = 0;
        currentLine[0] = '\0';
        
        // Read and optimize line by line
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

void processLine() {
    // This function is called by lexer but actual processing
    // is done in optimizeLine for simplicity
}

void invalidateExpressionsUsing(const char* varName);
void parseReadStatement(const char* line, char* varName);

void optimizeLine(char* line) {
    // Skip empty lines
    if (line[0] == '\n' || line[0] == '\0') return;
    
    // Check if it's a read statement
    if (strstr(line, "read(") != NULL) {
        char varName[64];
        parseReadStatement(line, varName);
        if (varName[0] != '\0') {
            invalidateExpressionsUsing(varName);
            printf("Read detected on '%s': invalidating expressions\n", varName);
        }
        return;
    }
    
    // Check if it's an assignment
    char lhs[64] = "";
    char rhs[256] = "";
    char temp[512];
    strcpy(temp, line);
    
    // Parse assignment: LHS = RHS
    char* eqPos = strchr(temp, '=');
    if (eqPos == NULL) return;
    
    // Extract LHS
    *eqPos = '\0';
    char* lhsStart = temp;
    while (*lhsStart == ' ' || *lhsStart == '\t') lhsStart++;
    char* lhsEnd = eqPos - 1;
    while (lhsEnd > lhsStart && (*lhsEnd == ' ' || *lhsEnd == '\t')) lhsEnd--;
    *(lhsEnd + 1) = '\0';
    strcpy(lhs, lhsStart);
    
    // Extract RHS
    char* rhsStart = eqPos + 1;
    while (*rhsStart == ' ' || *rhsStart == '\t') rhsStart++;
    char* rhsEnd = rhsStart + strlen(rhsStart) - 1;
    while (rhsEnd > rhsStart && (*rhsEnd == '\n' || *rhsEnd == ' ' || *rhsEnd == '\t')) rhsEnd--;
    *(rhsEnd + 1) = '\0';
    strcpy(rhs, rhsStart);
    
    // Check if RHS is a simple assignment (no operator)
    int isSimple = 1;
    for (int i = 0; rhs[i]; i++) {
        if (rhs[i] == '+' || rhs[i] == '-' || rhs[i] == '*' || 
            rhs[i] == '/' || rhs[i] == '%') {
            isSimple = 0;
            break;
        }
    }
    
    if (isSimple) {
        // Simple assignment, just record it
        addExpression(lhs, rhs);
        return;
    }
    
    // Check if this expression already exists
    char* existingVar = findExpression(rhs);
    if (existingVar != NULL) {
        // Replace with existing variable
        sprintf(line, "%s = %s\n", lhs, existingVar);
        printf("Optimized: %s = %s (CSE applied)\n", lhs, existingVar);
    } else {
        // New expression, add to list
        addExpression(lhs, rhs);
    }
}

void addExpression(const char* lhs, const char* rhs) {
    // Check if LHS already exists and update it
    ExprNode* current = exprList;
    while (current != NULL) {
        if (strcmp(current->lhs, lhs) == 0) {
            strcpy(current->rhs, rhs);
            return;
        }
        current = current->next;
    }
    
    // Add new expression
    ExprNode* newNode = (ExprNode*)malloc(sizeof(ExprNode));
    strcpy(newNode->lhs, lhs);
    strcpy(newNode->rhs, rhs);
    newNode->next = exprList;
    exprList = newNode;
}

char* findExpression(const char* rhs) {
    ExprNode* current = exprList;
    while (current != NULL) {
        if (strcmp(current->rhs, rhs) == 0) {
            return current->lhs;
        }
        current = current->next;
    }
    return NULL;
}

void freeExprList() {
    ExprNode* current = exprList;
    while (current != NULL) {
        ExprNode* next = current->next;
        free(current);
        current = next;
    }
    exprList = NULL;
}

void parseReadStatement(const char* line, char* varName) {
    varName[0] = '\0';
    
    // Find "read(" in the line
    const char* readPos = strstr(line, "read(");
    if (readPos == NULL) return;
    
    // Skip "read("
    const char* start = readPos + 5;
    
    // Find the variable name until ')' or whitespace
    int i = 0;
    while (start[i] != ')' && start[i] != '\0' && start[i] != ' ' && start[i] != '\t') {
        varName[i] = start[i];
        i++;
    }
    varName[i] = '\0';
}

void invalidateExpressionsUsing(const char* varName) {
    ExprNode* prev = NULL;
    ExprNode* current = exprList;
    
    while (current != NULL) {
        // Check if this expression uses the variable
        int usesVar = 0;
        
        // Check if varName appears in the RHS
        const char* pos = current->rhs;
        while ((pos = strstr(pos, varName)) != NULL) {
            // Check if it's a complete match (not part of another identifier)
            int len = strlen(varName);
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
            // Remove this node
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