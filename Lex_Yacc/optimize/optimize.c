#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./optimize.h"

expl_node* head = NULL;

// Create a new node
expl_node* createNode(char* op1, char* opr, char* op2, char* res) {
    expl_node* newNode = (expl_node*)malloc(sizeof(expl_node));
    newNode->operand1 = strdup(op1);
    newNode->operator = strdup(opr);
    newNode->operand2 = strdup(op2);
    newNode->result = strdup(res);
    newNode->next = NULL;
    return newNode;
}

// Lookup existing expression (returns pointer to node if found, else NULL)
expl_node* lookup(char* op1, char* opr, char* op2) {
    expl_node* temp = head;
    while (temp != NULL) {
        // exact match
        if (strcmp(temp->operand1, op1) == 0 &&
            strcmp(temp->operand2, op2) == 0 &&
            strcmp(temp->operator, opr) == 0) {
            return temp;
        }

        // commutative match for + and *
        if ((strcmp(opr, "+") == 0 || strcmp(opr, "*") == 0) &&
            strcmp(temp->operator, opr) == 0 &&
            strcmp(temp->operand1, op2) == 0 &&
            strcmp(temp->operand2, op1) == 0) {
            return temp;
        }

        temp = temp->next;
    }
    return NULL;
}

// Insert new expression only if it doesn't already exist
expl_node* insertExpr(char* op1, char* opr, char* op2, char* res) {
    // expl_node* existing = lookup(op1, opr, op2);
    // if (existing != NULL) {
    //     // Already exists return the existing node
    //     printf("Duplicate found: Using existing result %s\n", existing->result);
    //     return existing;
    // }

    expl_node* newNode = createNode(op1, opr, op2, res);
    if (head == NULL) {
        head = newNode;
    } else {
        expl_node* temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }

    return newNode;
}

void removeNodesWithOperand(char* variable) {
    expl_node* temp = head;
    expl_node* prev = NULL;

    while (temp != NULL) {
        // Check if either operand matches the variable
        if ((strcmp(temp->operand1, variable) == 0) ||
            (strcmp(temp->operand2, variable) == 0) ||
            (strcmp(temp->operator, variable) == 0))
            {

            // Remove the node
            if (prev == NULL) {
                // Deleting the head node
                head = temp->next;
                free(temp);
                temp = head;
            } else {
                prev->next = temp->next;
                free(temp);
                temp = prev->next;
            }
        } else {
            // Move forward
            prev = temp;
            temp = temp->next;
        }
    }
}

// Display all stored expressions
void displayList() {
    expl_node* temp = head;
    printf("\nCurrent Expression List:\n");
    while (temp != NULL) {
        printf("%s = %s %s %s\n", temp->result, temp->operand1, temp->operator, temp->operand2);
        temp = temp->next;
    }
}