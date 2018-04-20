
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint-gcc.h>

#include "utils.h"
#include "tables.h"

const int SYMTBL_NON_UNIQUE = 0;
const int SYMTBL_UNIQUE_NAME = 1;

/*******************************
 * Helper Functions
 *******************************/

void allocation_failed() {
    write_to_log("Error: allocation failed\n");
    exit(1);
}

void addr_alignment_incorrect() {
    write_to_log("Error: address is not a multiple of 4.\n");
}

void name_already_exists(const char* name) {
    write_to_log("Error: name '%s' already exists in table.\n", name);
}

void write_symbol(FILE* output, uint32_t addr, const char* name) {
    fprintf(output, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time. 
   If memory allocation fails, you should call allocation_failed(). 
   Mode will be either SYMTBL_NON_UNIQUE or SYMTBL_UNIQUE_NAME. You will need
   to store this value for use during add_to_table().
 */
SymbolTable* create_table(int mode) {
    SymbolTable* symbolTable = malloc(sizeof(symbolTable));
    if (symbolTable == NULL){
        allocation_failed();
    }
    symbolTable -> tbl = NULL;
    symbolTable -> len = 0;
    symbolTable -> cap = 0;
    symbolTable -> mode = mode;
    return symbolTable;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable* table) {
    for (int i = 0; i < table -> len; i++){
        free(table -> tbl[i].name);
    }
    free(table -> tbl);
    free(table);
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE. 
   ADDR is given as the byte offset from the first instruction. The SymbolTable
   must be able to resize itself as more elements are added. 

   Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.

   If ADDR is not word-aligned, you should call addr_alignment_incorrect() and
   return -1. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists 
   in the table, you should call name_already_exists() and return -1. If memory
   allocation fails, you should call allocation_failed(). 

   Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable* table, const char* name, uint32_t addr) {
    if (addr % 4 != 0){
        addr_alignment_incorrect();
        return -1;
    }
    if (table -> mode){
        for(int i = 0; i < table -> len; i++){
            if (strcmp((table -> tbl[i]).name, name) == 0){
                name_already_exists(name);
                return -1;
            }
        }
    }

    if (table -> cap == 0){
        Symbol* temp = malloc(sizeof(Symbol));
        if (temp == NULL){
            allocation_failed();
            return -1;
        }
        table -> cap = 1;
        table -> tbl = temp;
        temp = NULL;
    }

    if (table -> len == table -> cap){
        Symbol* temp = malloc((sizeof(Symbol) * table -> cap * 2));
        if (temp == NULL){
            allocation_failed();
            return -1;
        }
        for (int i = 0; i < (table -> len); i++){
            memcpy(&temp[i], &table -> tbl[i], sizeof(Symbol));
        }
        table -> cap *= 2;
        table -> tbl = temp;
        temp = NULL;
    }

    Symbol* temp = malloc(sizeof(Symbol));
    if (temp == NULL){
        allocation_failed();
        return -1;
    }
    temp -> name = malloc(sizeof(name));
    if (temp -> name == NULL){
        allocation_failed();
        return -1;
    }
    strcpy(temp -> name, name);
    temp -> addr = addr;
    memcpy(&table -> tbl[table -> len], temp, sizeof(Symbol));
    temp = NULL;
    table -> len += 1;

    return 0;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable* table, const char* name) {
    for(int i = 0; i < table -> len; i++){
        if (strcmp(table -> tbl[i].name, name) == 0){
            return table -> tbl[i].addr;
        }
    }
    return -1;   
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_symbol() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable* table, FILE* output) {
    for(int i = 0; i < table -> len; i++){
        write_symbol(output, table -> tbl[i].addr, table -> tbl[i].name);
    }
}
