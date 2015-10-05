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
int li_expansion(char** args, FILE* output) {
    // Check if can fit in imm field of an addiu instruction; optimization.
    //sdfsdfsdfchar* end; 
    //long int imm = strtol(args[1], &end, 0);
    int failure;
    long int imm_pointer; 
    failure = translate_num(&imm_pointer, args[1], INT32_MIN, UINT32_MAX);
    if (failure) {
      return 0; 
    }

    int num_instruct = 0;  
    if ((imm_pointer) >= INT16_MIN && (imm_pointer) <= UINT16_MAX) {
      // Make addiu instruction. 
      // addiu $dest $base value 
      // failure = write_addiu(9, output, args, num_args);
      fprintf(output, "addiu %s, $0, %li\n", args[0], imm_pointer);
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
      int lui_int = (imm_pointer) >> 16;
      uint32_t mask = 0x0000ffff; // for removing leading     1's
      lui_int &= mask; 
      fprintf(output, "lui $at, %i\n", lui_int);
      num_instruct++;

      // Okay should be fine; shifting won't change the original value. 
      //failure = write_ori(13, output, args, num_args);
      int ori_int = ((imm_pointer) << 16) >> 16;
      //uint32_t mask = 0x0000ffff; // for removing leading 1's
      ori_int &= mask; 
      fprintf(output, "ori $at, $at, %i\n", ori_int);
      num_instruct++;
      // if (!failure) {
      //     num_instruct++;
      // }
    }
    return num_instruct; 
    
}


int move_expansion(char** args, FILE* output) {
    // Move is like basically just addu. 
    // args: first is dest, 2nd is what is being copied
    // addiu args[0], args[1], $0; 
    //int failure;
    int num_instruct = 0;
    fprintf(output, "addiu %s, %s, $0\n", args[0], args[1]);
    num_instruct++;
    return num_instruct;
}


int blt_expansion(char** args, FILE* output) {
    // Want to use slt to get 1
    // And then use bne with zero
    //int failure;
    int num_instruct = 0;
    // Slt $at, args[0], args[1]
    // char** slt_args;
    // slt_args[0] = "$at";
    // slt_args[1] = args[0];
    // slt_args[2] = args[1];
    // failure = write_rtype(42, output, slt_args, 3);
    // if (!failure) {
    //   num_instruct++;
    // }

    fprintf(output, "slt $at, %s, %s\n", args[0], args[1]);
    num_instruct++;

    // slt would be 1 if indeed less than. Otherwise 0.

    // Now bne
    // bne $at, $0, args[2]
    // char** bne_args;
    // bne_args[0] = "$at";
    // bne_args[1] = "$0";
    // bne_args[2] = args[2];
    fprintf(output, "bne $at, $0, %s\n", args[2]);
    num_instruct++;

    return num_instruct; 
    // PC + 4. BUT HOW GET PC OMG
    //failure = write_branch(5, output, bne_args, 3, )
}

int bgt_expansion(char** args, FILE* output) {
    int num_instruct = 0;
    // Same as blt I think, just flip the registers when slt.
    // Just to make sure = case doesn't get caught up. 

    // slt $at, args[1], args[0]
    fprintf(output, "slt $at, %s, %s\n", args[1], args[0]);
    num_instruct++;

    // bne $at, $0, args[2]
    fprintf(output, "bne $at, $0, %s\n", args[2]);;
    num_instruct++;

    return num_instruct; 
}

int traddu_expansion(char** args, FILE* output) {
     int num_instruct = 0; 
     // Traddu is just a triple add instead of a double add. 
     // I'll just addu the first 2 and put into $at, then addu $at with the last one and put into final dest. 
     // addu $at, args[0], args[1]
     fprintf(output, "addu $at, %s, %s\n", args[0], args[1]);
     num_instruct++;

     // addu args[0], $at, args[2]
     fprintf(output, "addu %s, $at, %s", args[0], args[2]);
     num_instruct++; 

     return num_instruct; 
}

int swpr_expansion(char** args, FILE* output) {
    int num_instruct = 0;

    // Swaps the values. 
    // Save one value to $at, then just add. 

    // Save 1st value in $at. 
    // addu $at, arg[0], $0
    fprintf(output, "addu $at, %s, $0\n", args[0]);
    num_instruct++;

    // Now move 2nd to first.
    // addu arg[0], arg[1], $0
    fprintf(output, "addu %s, %s, $0\n", args[0], args[1]);
    num_instruct++;

    // Now move 1st value (stored in $at) to 2nd. 
    // addu args[1], $at, $0
    fprintf(output, "addu %s, $at, $0\n", args[1]);
    num_instruct++;

    return num_instruct;
}

int mul_expansion(char** args, FILE* output) {
    int num_instruct = 0;

    // Wants the lower 32 bits of the product - $lo. 

    // Becomes mult and mflo
    // Mult - hilo get the full 64 bits. Hi = upper, lo = lower. 
    // So if want the lower 32, just grab lo. 

    // mult args[1], args[2]
    fprintf(output, "mult %s, %s\n", args[1], args[2]);
    num_instruct++;

    // Wanted is in $lo now - move it
    // mflo args[0]
    fprintf(output, "mflo %s\n", args[0]);
    num_instruct++;

    return num_instruct; 
}

int div_expansion(char** args, FILE* output) {
    int num_instruct = 0;

    // Return quotient - in lo. 
    // div args[1] args[2]
    fprintf(output, "div %s, %s\n", args[1], args[2]);
    num_instruct++;
    // mflo args[0]
    fprintf(output, "mflo %s\n", args[0]);
    num_instruct++;

    return num_instruct; 
}

int rem_expansion(char** args, FILE* output) {
    int num_instruct = 0;

    // It's just like div, but grab from hi instead. 
    // div args[1] args[2]
    fprintf(output, "div %s, %s\n", args[1], args[2]);
    num_instruct++;
    // mfhi args[0]
    fprintf(output, "mfhi %s\n", args[0]);
    num_instruct++;

    return num_instruct; 
}






unsigned write_pass_one(FILE* output, const char* name, char** args, int num_args) {
    if (strcmp(name, "li") == 0) {
        /* YOUR CODE HERE */
        // li should take in 2 args. 
        if (check_num_args(num_args, 2) && li_can_rep_32(args)) {
            return li_expansion(args, output);
        }
        return 0; // For very primal bad errors. 
    } else if (strcmp(name, "move") == 0) {
        /* YOUR CODE HERE */
        // move should take in 2 args
        if (check_num_args(num_args, 2)) {
            return move_expansion(args, output);
        }
        return 0;  
    } else if (strcmp(name, "blt") == 0) {
        /* YOUR CODE HERE */
        if (check_num_args(num_args, 3)) {
            return blt_expansion(args, output);
        }
        return 0;  
    } else if (strcmp(name, "bgt") == 0) {
        /* YOUR CODE HERE */
        if (check_num_args(num_args, 3)) {
            return bgt_expansion(args, output);
        }
        return 0;  
    } else if (strcmp(name, "traddu") == 0) {
        /* YOUR CODE HERE */
        if (check_num_args(num_args, 3)) {
            return traddu_expansion(args, output);
        }
        return 0;       
    } else if (strcmp(name, "swpr") == 0) {
        /* YOUR CODE HERE */
        if (check_num_args(num_args, 2)) {
            return swpr_expansion(args, output);
        }
        return 0;       
    } else if (strcmp(name, "mul") == 0) {
        /* YOUR CODE HERE */
        if (check_num_args(num_args, 3)) {
            return mul_expansion(args, output);
        }
        return 0;       
    } else if (strcmp(name, "div") == 0) {
        /* YOUR CODE HERE */
        if (check_num_args(num_args, 3)) {
            return div_expansion(args, output);
        }
        return 0;       
    } else if (strcmp(name, "rem") == 0) {
        /* YOUR CODE HERE */
        if (check_num_args(num_args, 3)) {
            return rem_expansion(args, output);
        }
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
    else if (strcmp(name, "jr") == 0)   return write_jr (0x08, output, args, num_args);
    else if (strcmp(name, "addiu") == 0)   return write_addiu (0x9, output, args, num_args); 
    else if (strcmp(name, "ori") == 0)   return write_ori (0xD, output, args, num_args); 
    else if (strcmp(name, "lui") == 0)   return write_lui (0xf, output, args, num_args);
    else if (strcmp(name, "lb") == 0)   return write_mem (0x20, output, args, num_args);
    else if (strcmp(name, "lbu") == 0)   return write_mem (0x24, output, args, num_args);
    else if (strcmp(name, "lw") == 0)   return write_mem (0x23, output, args, num_args);
    else if (strcmp(name, "sb") == 0)   return write_mem (0x28, output, args, num_args);
    else if (strcmp(name, "sw") == 0)   return write_mem (0x2b, output, args, num_args);
    else if (strcmp(name, "beq") == 0)   return write_branch (0x4, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "bne") == 0)   return write_branch (0x5, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "j") == 0)   return write_jump (0x2, output, args, num_args, addr, reltbl);
    else if (strcmp(name, "jal") == 0)   return write_jump (0x3, output, args, num_args, addr, reltbl);
    else if (strcmp(name, "mult") == 0)   return write_rtype (0x18, output, args, num_args);
    else if (strcmp(name, "div") == 0)   return write_rtype (0x1a, output, args, num_args);
    else if (strcmp(name, "mfhi") == 0)   return write_rtype (0x10, output, args, num_args);
    else if (strcmp(name, "mflo") == 0)   return write_rtype (0x12, output, args, num_args);
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
    if (num_args != 3) {
        return -1;
    }

    int rd = translate_reg(args[0]);
    int rs = translate_reg(args[1]);
    int rt = translate_reg(args[2]);

    uint32_t instruction = 0;
    // Combine rd, rs, and rt into binary representation, then use write_inst_hex to translate to hex
    // ORDER: opcode=0, rs, rt, rd, shamt=0, func
    instruction += funct;
    instruction += (rd << 11);
    instruction += (rt << 16);
    instruction += (rs << 21);
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
    if (num_args != 3) {
        return -1;
    }
    long int shamt;
    int rd = translate_reg(args[0]);
    int rt = translate_reg(args[1]);
    int err = translate_num(&shamt, args[2], 0, 31);

    // Only continue if translate_num doesn't result in an error
    if (err != -1) {
        uint32_t instruction = 0;
        // ORDER: opcode=0, rs=unused, rt, rd, shamt, funct
        instruction += funct;
        instruction += (shamt << 6);
        instruction += (rd << 11);
        instruction += (rt << 16);
        write_inst_hex(output, instruction);
        return 0;
    }
    else { return -1; }
}

/* The rest of your write_*() functions below */

int write_jr(uint8_t funct, FILE* output, char** args, size_t num_args) {
    // Perhaps perform some error checking?
    if (num_args != 1) {
        return -1;
    }
    int rs = translate_reg(args[0]);
    // ORDER: opcode=0, rs, rt=0, rd=0, shamt=0, funct
    uint32_t instruction = 0;
    instruction += funct;
    instruction += (rs << 21);
    write_inst_hex(output, instruction);
    return 0;
}

int write_addiu(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    // Perhaps perform some error checking?
    if (num_args != 3) {
        return -1;
    }    
    long int imm;
    int rt = translate_reg(args[0]);
    int rs = translate_reg(args[1]);
    // need clarification on addiu - what does it mean to pass in a negative immediate?
    int err = translate_num(&imm, args[2], INT16_MIN, INT16_MAX);
    if (err != -1) {
        uint32_t instruction = 0;
        // ORDER: opcode, rs, rt, imm
        uint32_t mask = 0x0000ffff; // for removing leading 1's
        imm &= mask;
        instruction += imm;
        instruction += (rt << 16);
        instruction += (rs << 21);
        instruction += (opcode << 26);
        write_inst_hex(output, instruction);
        return 0;
    } 
    else { return -1; }
}

int write_ori(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    // Perhaps perform some error checking?
    if (num_args != 3) {
        return -1;
    }
    long int imm;
    int rt = translate_reg(args[0]);
    int rs = translate_reg(args[1]);
    int err = translate_num(&imm, args[2], 0, UINT16_MAX);
    if (err != -1) {
        uint32_t instruction = 0;
        // ORDER: opcode, rs, rt, imm
        instruction += imm;
        instruction += (rt << 16);
        instruction += (rs << 21);
        instruction += (opcode << 26);
        write_inst_hex(output, instruction);
        return 0;
    }
    else { return -1; }
}

int write_lui(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    // Perhaps perform some error checking?
    if (num_args != 2) {
        return -1;
    }
    long int imm;
    int rt = translate_reg(args[0]);
    int err = translate_num(&imm, args[1], 0, UINT16_MAX);

    if (err != -1) {
        uint32_t instruction = 0;
        // ORDER: opcode, rs=0, rt, imm
        instruction += imm;
        instruction += (rt << 16);
        instruction += (opcode << 26);
        write_inst_hex(output, instruction);
        return 0;
    }
    else { return -1; }
}

int write_mem(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    // Perhaps perform some error checking?
    if (num_args != 3) {
        return -1;
    }
    long int imm;
    int rt = translate_reg(args[0]);
    int rs = translate_reg(args[2]);
    int err = translate_num(&imm, args[1], INT16_MIN, INT16_MAX);

    if (err != -1) {
        uint32_t instruction = 0;
        // ORDER: opcode, rs, rt, imm
        uint32_t mask = 0x0000ffff; // for removing leading 1's
        imm &= mask;
        instruction += imm;
        instruction += (rt << 16);
        instruction += (rs << 21);
        instruction += (opcode << 26);
        write_inst_hex(output, instruction);
        return 0;
    }
    else { return -1; }
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
    if (num_args != 3) {
        return -1;
    }
    int rs = translate_reg(args[0]);
    int rt = translate_reg(args[1]);
    int label_addr = get_addr_for_symbol(symtbl, args[2]);
    // Need to check if can_branch_to and if offset fits inside the immediate field
    // assuming that addr is the PC address
    // Check if branch offset fits inside immediate field
    if (!can_branch_to(addr, label_addr) || ((label_addr << 16) >> 16 != label_addr)) {
        return -1;
    }
    int32_t offset = (int32_t)(label_addr - (addr + 4)) / 4;
    uint32_t mask = 0x0000ffff;
    offset &= mask;
    uint32_t instruction = 0;
    instruction += offset;
    instruction += (rt << 16);
    instruction += (rs << 21);
    instruction += (opcode << 26);
    write_inst_hex(output, instruction);
    return 0;
}

int write_jump(uint8_t opcode, FILE* output, char** args, size_t num_args, uint32_t addr, SymbolTable* reltbl) {
    if (num_args != 1) {
        return -1;
    }
    // Need absolute address of the label to jump to
    int label_addr = get_addr_for_symbol(reltbl, args[0]);
    if (label_addr == -1) {
        // if no address is found, set the address to 0
        label_addr = 0;
    }
    add_to_table(reltbl, args[0], addr);
    uint32_t instruction = 0;
    // ORDER: opcode, addr
    uint32_t mask = 0x03ffffff;
    label_addr &= mask;
    instruction += label_addr;
    instruction += (opcode << 26);
    write_inst_hex(output, instruction);
    return 0;
}
