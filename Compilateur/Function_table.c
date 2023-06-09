#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Function_table.h"

#define TABLE_SIZE 100


// Déclaration de la table de fonctions
function_line *function_table[TABLE_SIZE];

// Déclaration de l'indice courant dans la table de fonctions
int current_function_index = 0;

// Fonction d'initialisation de la table de fonctions
void init_function_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        function_table[i] = NULL;
    }
}

// Fonction d'ajout d'une fonction à la table de fonctions
void add_function(char *name, int asm_address, int params_count) {
    function_line *line;
    for (int i = 0; i < TABLE_SIZE; i++) {
        line = function_table[i];
        if (line && !strcmp(line->name, name)) {
            printf("Function '%s' already exists in the function table\n", name);
            return;
        }
    }

    function_line *new_function = (function_line *) malloc(sizeof(function_line));
    new_function->name = strdup(name);
    new_function->asm_address = asm_address;
    new_function->params_count = params_count;
    function_table[current_function_index] = new_function;
    current_function_index++;
}


// Fonction de récupération de l'adresse assembleur (asm) d'une fonction dans la table de fonctions
int find_function_asm_address(char *name) {
    function_line *line;
    for (int i = 0; i < TABLE_SIZE; i++) {
        line = function_table[i];
        if (line && !strcmp(line->name, name)) {
            return line->asm_address;
        }
    }
    return -1;
}

int get_function_params(char *name) {
    function_line *line;
    for (int i = 0; i < TABLE_SIZE; i++) {
        line = function_table[i];
        if (line && !strcmp(line->name, name)) {
            return line->params_count;
        }
    }
    return -1;
}

void print_function_table() {
    printf("Table de fonctions :\n");
    printf("--------------------\n");
    for (int i = 0; i < current_function_index; i++) {
        printf("%d\t%s\t%d\t%d\n", i+1, function_table[i]->name, function_table[i]->asm_address,function_table[i]->params_count );
    }
    printf("--------------------\n");
}

#if 0

// Exemple d'utilisation
int main() {
    init_function_table();

    add_function("function1", 100, 3);
    add_function("function2", 200, 2);
    add_function("function3", 300, 1);
    add_function("function1", 400, 4);

    printf("Address of function1: %d\n", find_function_asm_address("function1"));
    printf("Params count of function1: %d\n", get_function_params("function1"));
    printf("Address of function2: %d\n", find_function_asm_address("function2"));
    printf("Params count of function2: %d\n", get_function_params("function2"));
   
}
#endif