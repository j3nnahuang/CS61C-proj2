
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "tables.h"

const int SYMTBL_NON_UNIQUE = 0;
const int SYMTBL_UNIQUE_NAME = 1;

#define INITIAL_SIZE 5
#define SCALING_FACTOR 2

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

/* Creates a new SymbolTable containing 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time. 
   If memory allocation fails, you should call allocation_failed(). 
   Mode will be either SYMTBL_NON_UNIQUE or SYMTBL_UNIQUE_NAME. You will need
   to store this value for use during add_to_table().
 */
SymbolTable* create_table(int mode) {
    /* YOUR CODE HERE */
    // So SymbolTable's tbl field = an ARRAY of Symbols. 

    // Thank goodness for lab02. 
    SymbolTable* table_pointer;

    /* First allocate memory for the struct. */
    table_pointer = malloc(sizeof(SymbolTable));

    /* Check return value - make sure got memory. */
    if (table_pointer == NULL) {
      allocation_failed();
    }

    /* Initialize all the fields. */
    table_pointer->tbl = malloc(INITIAL_SIZE * sizeof(Symbol)); 

    /* Check return value of Symbol ARRAY to make sure we got memory. */
    if (table_pointer->tbl == NULL) {
      free(table_pointer);
      allocation_failed();
    }

    table_pointer->len = 0;
    table_pointer->cap = INITIAL_SIZE; 
    table_pointer->mode = mode; 
    // Wow that's confusing. Mode field of SymbolTable = what was passed in. 

    return table_pointer; 
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable* table) {
    /* YOUR CODE HERE */
    // Okay gotta free the array of Symbols, the Symbols in the array, and then the Symbol Table itself. 
    // According to lab2, don't have to free the pointer to the table though, because on the stack, so will get freed. 


    // Ok I think don't need to free the Symbols in the array, b/c weren't malloced. 
    // Malloc - array would be an array of pointers to Symbols. 
    // But it's only got 1 star. 
    // So I'm assuming no malloc. 
    // So don't need to free the Symbols themselves inside the array. 

    // Free array of Symbols. 
    free(table->tbl);

    // Free symbol table itself. 
    free(table);
}

/* A suggested helper function for copying the contents of a string. */
static char* create_copy_of_str(const char* str) {
    size_t len = strlen(str) + 1;
    char *buf = (char *) malloc(len);
    if (!buf) {
       allocation_failed();
    }
    strncpy(buf, str, len); 
    return buf;
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
    /* YOUR CODE HERE */

    // WORD-ALIGNMENT
    // Word alignment, just divisible by 4. 
    if (addr % 4) {
      addr_alignment_incorrect();
      return -1; 
    }

    // UNIQUE-NAME
    if (table->mode && (get_addr_for_symbol(table, name) != -1)) {
      name_already_exists(name);
      return -1; 
    }

    // RESIZE
    int table_length = table->len;
    int table_cap = table->cap;
    if (table_length >= table_cap) {
      // LOL WAS HERE CONTINUE ALICE. 
    }


    return -1;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable* table, const char* name) {
    /* YOUR CODE HERE */
    // Okay hopefully lab02 is good way to go.
    if (table != NULL) {
        // Looking for the name
	     // I think have to loop through the entire array to check if name.
       int i = 0;
	     int array_size = table->len; 
	     // Hopefully will give me size of the array. 
        while (i < array_size) {
   	      if (!strcmp(name, table->tbl[i].name)) {
	        // Yes we've found the one. 
	        return table->tbl[i].addr;
 	        }
	        i++; 
	      }
    }	 
    return -1;
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_symbol() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable* table, FILE* output) {
    /* YOUR CODE HERE */
}
