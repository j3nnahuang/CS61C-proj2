#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tables.h"
#include "translate_utils.h"
#include "translate.h"

/* SOLUTION CODE BELOW */
const int TWO_POW_SEVENTEEN = 131072;    // 2^17

/* Writes instructions during the assembler's first pass to OUTPUT. The case
   for general instructions has already been completed, but you need to write
   code to translate the li and blt pseudoinstructions. Your pseudoinstruction 
   expansions should not have any side effects.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.

   Error checking for regular instructions are done in pass two. However, for
   pseudoinstructions, you must make sure that ARGS contains the correct number
   of arguments. You do NOT need to check whether the registers / label are 
   valid, since that will be checked in part two.

   Also for li:
    - make sure that the number is representable by 32 bits. (Hint: the number 
        can be both signed or unsigned).
    - if the immediate can fit in the imm field of an addiu instruction, then
        expand li into a single addiu instruction. Otherwise, expand it into 
        a lui-ori pair.
        

   For move, blt, bgt, traddu, swpr:
    - your expansion should use the fewest number of instructions possible.

   MARS has slightly different translation rules for li, and it allows numbers
   larger than the largest 32 bit number to be loaded with li. You should follow
   the above rules if MARS behaves differently.

   Use fprintf() to write. If writing multiple instructions, make sure that 
   each instruction is on a different line.

   Returns the number of instructions written (so 0 if there were any errors).
 */


int check_num_args(int num_args, int correct_num_args) {
    if (num_args == correct_num_args) {
      return 1;
    }
    return 0; 
}

int li_can_rep_32 (char** args) {
    // Range from min value of signed int to max value of unsigned ints
    // Immediate should be 2nd arg.
    // **args here, apparently, is an array of strings. 
    // Array of char*s = array of strings.
    char* end;  
    long int imm = strtol(args[1], &end, 0);
    if (imm >= INT32_MIN && imm <= UINT32_MAX) {
        return 1;
    }
    return 0; 
}

/* Should return # instructions written. */
int li_expansion(char** args, int num_args, FILE* output) {
    // Check if can fit in imm field of an addiu instruction; optimization.
    char* end; 
    //long int imm = strtol(args[1], &end, 0);
    int failure;
    long int* imm_pointer; 
    failure = translate_num(imm_pointer, args[1], INT32_MIN, UINT32_MAX);
    if (failure) {
      return 0; 
    }

    int num_instruct = 0;  
    if ((*imm_pointer) >= INT16_MIN && (*imm_pointer) <= UINT16_MAX) {
      // Make addiu instruction. 
      // addiu $dest $base value 
      // failure = write_addiu(9, output, args, num_args);
      fprintf(output, "addiu %s, $0, %li\n", args[0], *imm_pointer);
      num_instruct++;

      // if (!failure) {
      //   num_instruct++;
      // }
    } else {

      // Otherwise expand into lui-ori.
      // lui $at, num
      // ori $at, $at, immediate
      // addu $dest, $dest, $at

      //failure = write_lui(15, output, args, num_args);
      // Want first 16 bits = logical shift 
      int lui_int = (*imm_pointer) >> 16; 
      fprintf(output, "lui $at, %i\n", lui_int);
      num_instruct++;

      // Okay should be fine; shifting won't change the original value. 
      //failure = write_ori(13, output, args, num_args);
      int ori_int = (*imm_pointer) << 16; 
      fprintf(output, "ori $at, $at, %i\n", ori_int);
      num_instruct++;
      // if (!failure) {
      //     num_instruct++;
      // }
    }
    return num_instruct; 
    
}


int move_expansion(char** args, int num_args, FILE* output) {
    // Move is like basically just addu. 
    int failure;
    int num_instruct = 0;
    failure = write_rtype(33, output, args, num_args);
    if (!failure) {
        num_instruct++;
    }
    return num_instruct;
}


int blt_expansion(char** args, int num_args, FILE* output) {
    // Want to use slt to get 1
    // And then use bne with zero
    int failure
    int num_instruct = 0;
    // Slt $at, arg[0], arg[1]
    char** slt_args;
    slt_args[0] = "$at"
    slt_args[1] = args[0];
    slt_args[2] = args[1];
    failure = write_rtype(42, output, slt_args, 3);
    if (!failure) {
      num_instruct++;
    }
    // slt would be 1 if indeed less than. Otherwise 0.

    // Now bne
    // bne $at, $0, args[2]
    char** bne_args;
    bne_args[0] = "$at";
    bne_args[1] = "$0";
    bne_args[2] = args[2];
    // PC + 4. BUT HOW GET PC OMG
    failure = write_branch(5, output, bne_args, 3, )
}






unsigned write_pass_one(FILE* output, const char* name, char** args, int num_args) {
    if (strcmp(name, "li") == 0) {
        /* YOUR CODE HERE */
        // li should take in 2 args. 
        if (check_num_args(num_args, 2) && li_can_rep_32(args)) {
            return li_expansion(args, num_args, output);
        }
        return 0; // For very primal bad errors. 
    } else if (strcmp(name, "move") == 0) {
        /* YOUR CODE HERE */
        // move should take in 2 args
        if (check_num_args(num_args, 2)) {
            return move_expansion(args, num_args, output);
        }
        return 0;  
    } else if (strcmp(name, "blt") == 0) {
        /* YOUR CODE HERE */
        if (check_num_args(num_args, 3)) {
            STUFFFFF
        }
        return 0;  
    } else if (strcmp(name, "bgt") == 0) {
        /* YOUR CODE HERE */
        return 0;  
    } else if (strcmp(name, "traddu") == 0) {
        /* YOUR CODE HERE */
        return 0;       
    } else if (strcmp(name, "swpr") == 0) {
        /* YOUR CODE HERE */
        return 0;       
    } else if (strcmp(name, "mul") == 0) {
        /* YOUR CODE HERE */
        return 0;       
    } else if (strcmp(name, "div") == 0) {
        /* YOUR CODE HERE */
        return 0;       
    } else if (strcmp(name, "rem") == 0) {
        /* YOUR CODE HERE */
        return 0;       
    } 
    write_inst_string(output, name, args, num_args);
    return 1;

}



/* Writes the instruction in hexadecimal format to OUTPUT during pass #2.
   
   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS. 

   The symbol table (SYMTBL) is given for any symbols that need to be resolved
   at this step. If a symbol should be relocated, it should be added to the
   relocation table (RELTBL), and the fields for that symbol should be set to
   all zeros. 

   You must perform error checking on all instructions and make sure that their
   arguments are valid. If an instruction is invalid, you should not write 
   anything to OUTPUT but simply return -1. MARS may be a useful resource for
   this step.

   Some function declarations for the write_*() functions are provided in translate.h, and you MUST implement these
   and use these as function headers. You may use helper functions, but you still must implement
   the provided write_* functions declared in translate.h.

   Returns 0 on success and -1 on error. 
 */
int translate_inst(FILE* output, const char* name, char** args, size_t num_args, uint32_t addr,
    SymbolTable* symtbl, SymbolTable* reltbl) {
    if (strcmp(name, "addu") == 0)       return write_rtype (0x21, output, args, num_args);
    else if (strcmp(name, "or") == 0)    return write_rtype (0x25, output, args, num_args);
    else if (strcmp(name, "slt") == 0)   return write_rtype (0x2a, output, args, num_args);
    else if (strcmp(name, "sltu") == 0)  return write_rtype (0x2b, output, args, num_args);
    else if (strcmp(name, "sll") == 0)   return write_shift (0x00, output, args, num_args);
    /* YOUR CODE HERE */
    else                                 return -1;
}

/* A helper function for writing most R-type instructions. You should use
   translate_reg() to parse registers and write_inst_hex() to write to 
   OUTPUT. Both are defined in translate_utils.h.

   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_rtype(uint8_t funct, FILE* output, char** args, size_t num_args) {
    // Perhaps perform some error checking?

    int rd = translate_reg(args[0]);
    int rs = translate_reg(args[1]);
    int rt = translate_reg(args[2]);

    uint32_t instruction = 0;
    write_inst_hex(output, instruction);
    return 0;
}

/* A helper function for writing shift instructions. You should use 
   translate_num() to parse numerical arguments. translate_num() is defined
   in translate_utils.h.

   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_shift(uint8_t funct, FILE* output, char** args, size_t num_args) {
	// Perhaps perform some error checking?

    long int shamt;
    int rd = translate_reg(args[0]);
    int rt = translate_reg(args[1]);
    int err = translate_num(&shamt, args[2], 0, 31);

    uint32_t instruction = 0;
    write_inst_hex(output, instruction);
    return 0;
}

/* The rest of your write_*() functions below */

int write_jr(uint8_t funct, FILE* output, char** args, size_t num_args) {
    // Perhaps perform some error checking?

    int rs = translate_reg(args[0]);

    uint32_t instruction = 0;
    write_inst_hex(output, instruction);
    return 0;
}

int write_addiu(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    // Perhaps perform some error checking?
    
    long int imm;
    int rt = translate_reg(args[0]);
    int rs = translate_reg(args[1]);
    int err = translate_num(&imm, args[2], INT16_MIN, INT16_MAX);


    uint32_t instruction = 0;
    write_inst_hex(output, instruction);
    return 0;
}

int write_ori(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    // Perhaps perform some error checking?
    
    long int imm;
    int rt = translate_reg(args[0]);
    int rs = translate_reg(args[1]);
    int err = translate_num(&imm, args[2], 0, UINT16_MAX);

    uint32_t instruction = 0;
    write_inst_hex(output, instruction);
    return 0;
}

int write_lui(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    // Perhaps perform some error checking?
    
    long int imm;
    int rt = translate_reg(args[0]);
    int err = translate_num(&imm, args[1], 0, UINT16_MAX);

    uint32_t instruction = 0;
    write_inst_hex(output, instruction);
    return 0;
}

int write_mem(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    // Perhaps perform some error checking?
    
    long int imm;
    int rt = translate_reg(args[0]);
    int rs = translate_reg(args[2]);
    int err = translate_num(&imm, args[1], INT16_MIN, INT16_MAX);

    uint32_t instruction =0;
    write_inst_hex(output, instruction);
    return 0;
}

/*  A helper function to determine if a destination address
    can be branched to
*/
static int can_branch_to(uint32_t src_addr, uint32_t dest_addr) {
    int32_t diff = dest_addr - src_addr;
    return (diff >= 0 && diff <= TWO_POW_SEVENTEEN) || (diff < 0 && diff >= -(TWO_POW_SEVENTEEN - 4));
}


int write_branch(uint8_t opcode, FILE* output, char** args, size_t num_args, uint32_t addr, SymbolTable* symtbl) {
    // Perhaps perform some error checking?
    
    int rs = translate_reg(args[0]);
    int rt = translate_reg(args[1]);
    int label_addr = get_addr_for_symbol(symtbl, args[2]);

    int32_t offset = 0;
    uint32_t instruction = 0;
    write_inst_hex(output, instruction);        
    return 0;
}

int write_jump(uint8_t opcode, FILE* output, char** args, size_t num_args, uint32_t addr, SymbolTable* reltbl) {
    /* YOUR CODE HERE */
    
    uint32_t instruction = 0;
    write_inst_hex(output, instruction);
    return 0;
}
