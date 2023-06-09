#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Symbol_table.h"

int address = 0;

// On initialise une table
symbol_table *init_symbol_table() {
    symbol_table *table = (symbol_table*) malloc(sizeof(symbol_table));
    table->first_entry = NULL;
    table->last_entry = NULL;
    return table;
}

// ajoute une variable temporaire 
int add_temp_var(symbol_table *table){
 // on creer une nouvelle entrée    
    symbol_table_entry *entry = table->first_entry;
    entry = (symbol_table_entry*) malloc(sizeof(symbol_table_entry));
    entry->name = strdup("");
    entry->address = address++;
    entry->next = NULL;

// on ajoute l'entrée
    if (table->first_entry == NULL) {
        table->first_entry = entry;
        table->last_entry = entry;
    } else {
        table->last_entry->next = entry;
        table->last_entry = entry;
    }
    return(entry->address);
}

//ajoute l'adresse de retour 

int add_return_address(symbol_table *table, int decalage){
 // on creer une nouvelle entrée    
    symbol_table_entry *entry = table->first_entry;
    entry = (symbol_table_entry*) malloc(sizeof(symbol_table_entry));
    entry->name = strdup("@");
    entry->address = address++;
    entry ->decalage = decalage;
    entry->next = NULL;

// on ajoute l'entrée
    if (table->first_entry == NULL) {
        table->first_entry = entry;
        table->last_entry = entry;
    } else {
        table->last_entry->next = entry;
        table->last_entry = entry;
    }
    return(entry->address);
}

// free la variable temporaire 
void remove_last_temp_var(symbol_table *table) {
    if (table->first_entry == NULL) {
        // la table est vide
        fprintf(stderr, "Symbol table is empty\n");
        exit(1);
    }
    if (table->last_entry->name != NULL && strcmp(table->last_entry->name, "") != 0) {
        // la dernière entrée n'est pas une variable temporaire
        fprintf(stderr, "Last symbol is not a temporary variable\n");
        exit(1);
    }
    symbol_table_entry *entry = table->first_entry;
    symbol_table_entry *prev = NULL;
    while (entry != NULL) {
        if (entry == table->last_entry) {
            // on a trouvé la dernière entrée
            if (prev == NULL) {
                // c'est la seule entrée dans la table
                table->first_entry = NULL;
                table->last_entry = NULL;
            } else {
                // on supprime l'entrée
                prev->next = NULL;
                table->last_entry = prev;
            }
            address = entry->address;
            free(entry->name);
            free(entry);
            break;
        }
        prev = entry;
        entry = entry->next;
    }
}


//free les symboles du meme scope 
void remove_symbols_by_scope(symbol_table *table, int scope) {
    symbol_table_entry *entry = table->first_entry;
    symbol_table_entry *prev = NULL;
    while (entry != NULL) {
        if (entry->scope == scope) {
            if (prev == NULL) {
                // L'élément à supprimer est en première position
                table->first_entry = entry->next;
            } else {
                prev->next = entry->next;
            }
            if (entry == table->last_entry) {
                // L'élément à supprimer est en dernière position
                table->last_entry = prev;
            }
            
            symbol_table_entry *tmp = entry;
            entry->address=address--;
            entry = entry->next;
            free(tmp->name);
            free(tmp);
        } else {
            prev = entry;
            entry = entry->next;
        }
    }
}



// ajouter une entree a la table 
int add_symbol(symbol_table *table, char *name, int scope) {
    // on verifie s'il existe deja 
    symbol_table_entry *entry = table->first_entry;
    while (entry != NULL) {
        if (strcmp(entry->name, name) == 0) {
            fprintf(stderr, "Symbol %s already defined\n", name);
            exit(1);
        }
        entry = entry->next;
    }

    // on creer une nouvelle entrée 
    entry = (symbol_table_entry*) malloc(sizeof(symbol_table_entry));
    entry->name = strdup(name);
    entry->address = address++;
    entry->scope = scope;
    entry->next = NULL;

    // on ajoute l'entrée
    if (table->first_entry == NULL) {
        table->first_entry = entry;
        table->last_entry = entry;
    } else {
        table->last_entry->next = entry;
        table->last_entry = entry;
    }
    return entry->address;
}

// On regarde si le symbole est present et on renvoie son adresse 
int get_adress(symbol_table *table, char *name) {
    symbol_table_entry *entry = table->first_entry;
    while (entry != NULL) {
        if (strcmp(entry->name, name) == 0) {
            return entry->address;
        }
        entry = entry->next;
    }
    fprintf(stderr, "Symbol %s not defined\n", name);
    exit(1);
}

// libere la memoire utilisée par la table 
void free_table(symbol_table *table) {
    symbol_table_entry *entry = table->first_entry;
    while (entry != NULL) {
        symbol_table_entry *next = entry->next;
        free(entry->name);
        free(entry);
        entry = next;
    }
    table->first_entry = NULL;
    table->last_entry = NULL;
    address = 0;
}

// affiche la table 
void print_table(symbol_table *table) {
    printf("Symbol table:\n");
    symbol_table_entry *entry = table->first_entry;
    while (entry != NULL) {
        printf("%s\t%d\t%d\t%d\n", entry->name, entry->address,entry->scope, entry->decalage);
        entry = entry->next;
    }
}

int get_last_index_ts(symbol_table *table) {
    if (table->last_entry == NULL) {
        fprintf(stderr, "Symbol table is empty\n");
        exit(1);
    }
    return table->last_entry->address;
}

#if 0
int main() {
    symbol_table *table = init_symbol_table();
    add_symbol(table,"x",1);
    int addr1 = add_temp_var(table);
    printf ("address var temp 1 est : %d\n",addr1);
    int addr2 = add_temp_var(table);
    printf ("address var temp 2 est : %d\n",addr2);
    remove_last_temp_var(table);
    add_symbol(table,"d",2);
    add_symbol(table,"e",2);
    add_symbol(table,"t",3);
    add_symbol(table,"y",3);
    add_symbol(table,"u",4);
    add_symbol(table,"i",4);
    print_table(table);
    remove_symbols_by_scope(table,4);
    add_symbol(table,"b",1);
    print_table(table);
    add_return_address(table,322222);
    print_table(table);

}

// main pour tester les fonctions valide
int main() {
    symbol_table *table = init_symbol_table();
    // Add some symbols to the table
    add_symbol(table, "x", 1);
   
    add_symbol(table, "z", 1);
   
    int addr2 =  add_symbol(table, "y", 2);
     int addr1 = add_temp_var(table);
    // Look up some symbols in the table
    printf("Address of x: %d\n", get_adress(table, "x"));
    printf("Address of y: %d\n", get_adress(table, "y"));
    printf("Address of z: %d\n", get_adress(table, "z"));
    printf("adresse de la var temp 1 est : %d\n ", addr1);
    // Print the contents of the symbol table
    print_table(table);
    int addr_dernier = get_last_index_ts(table);
    printf("addresse dernier : %d\n",addr_dernier);
    remove_last_temp_var(table);
    printf("table apres remove scope 1\n");
    print_table(table); 
   
    printf("table apres remove last temp :\n");
    print_table(table);
    int addr3 = add_temp_var(table);
    printf("adresse de la var temp 2 est : %d\n ", addr3);
    printf("table apres remove var 1\n");
    print_table(table);

    printf("table apres remove var 2\n");
    print_table(table);
    // Free the memory used by the symbol table
    free_table(table);

    print_table(table);
}

#endif