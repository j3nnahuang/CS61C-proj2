# CS 61C Spring 2015 Project 2-2 
# symbol_list.s

#==============================================================================
#                              Project 2-2 Part 2
#                               SymbolList README
#==============================================================================
# In this file you will be implementing a linked list-based data structure for
# storing symbols. Its purpose is analogous to SymbolTable from Proj2-1, but to
# disinguish it we will call it SymbolList.
#
# Each node in the SymbolList contains a (name, addr) pair. An empty SymbolList
# is simply a pointer to NULL. As (name, addr) pairs are added to a SymbolList,
# new list nodes are dynamically allocated and appended to the front of the list. 
# However, as there is no free() in MARS, you do not need to write a free_list()
# function. You may use the functions in string.s during your implementation.
#
# You do not need to perform error checking on duplicate addr or name entries
# in SymbolList, nor do you need to do alignment checking. Don't worry about
# provided addresses being too big.
#
# If SymbolList were to be written in C, each list node would be declared as 
# follows:
#   typedef struct symbollist { 
#       char* name;
#       int addr;
#       struct symbollist* next; 
#   } SymbolList;
#
# You need to write addr_for_symbol() and add_to_list().
# You will find test cases in linker-tests/test_symbol_list.s
#==============================================================================

.include "string.s"

.text

#------------------------------------------------------------------------------
# function addr_for_symbol()
#------------------------------------------------------------------------------
# Iterates through the SymbolList and searches for an entry with the given name.
# If an entry is found, return that addr. Otherwise return -1.
#
# Arguments:
#  $a0 = pointer to a SymbolList (NULL indicates empty list)
#  $a1 = name to look for
#
# Returns:  address of symbol if found or -1 if not found
#------------------------------------------------------------------------------
addr_for_symbol:
    # YOUR CODE HERE
    addiu $sp $sp -12 # 3 things
    sw $ra 0($sp)
    sw $s0 4($sp)
    sw $s1 8($sp)
    # sw $a0 4($sp)
   	# Begin addr_for_symbol
    addu $s1 $a1 $0
   	
 looping_part:
 	addu $s0 $a0 $0 
 	
    #sw $a0 4($sp)
    #lw $a1 8($sp)
   	# First check if pointer is null - empty list. 
   	beq $a0, $0, addr_not_found
   	# Else, start checking out symbols. 
   	# I guess just check the addresses, because can't like load in a string. 
   	lw $t0, 0($a0) # $t0 holds curr name. 
    # Check if strings are equal
    addu $a0, $t0, $0 
    jal streq
    	addu $a0 $s0 $0
    	#addu $a1 $s1 $0
    #addu $a0 $s0 $0    
   	beq $v0, $0, addr_found 
   	# If not the right one, then load in next node and try again.
    # lw $a0, 4($sp)
   	lw $a0, 8($s0)
   	addu $a1 $s1 $0
   	j looping_part

    
       	
addr_found:
	# Grab the address of the current node. 
	lw $v0, 4($a0) 
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    addiu $sp $sp 12
	jr $ra
       	
addr_not_found: 
	li $v0, -1
    lw $ra 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    addiu $sp $sp 12
    jr $ra			# End addr_for_symbol
        
#------------------------------------------------------------------------------
# function add_to_list()
#------------------------------------------------------------------------------
# Adds a (name, addr) pair to the FRONT of the list. You should call new_node()
# to create a new node. You do not need to perform error checking on duplicate 
# addr or name entries in SymbolList, nor do you need to do alignment checking.
#
# As with Project 2-1, you WILL need to create a copy of the string that was
# passed in. copy_of_str() from Part 1 will be useful. After new entry has been
# added to the list, return the new list.
#
# Arguments:
#   $a0 = ptr to list (may be NULL)
#   $a1 = pointer to name of symbol (string)
#   $a2 = address of symbol (integer)
#
# Returns: the new list
#------------------------------------------------------------------------------
add_to_list:    
        # YOUR CODE HERE
        # Begin fxn add_to_list
        
        # List just has pointer to next - use $a0 as next. 
        
        # First create new node. 
        # Gotta call another fxn - save the args and $ra. AND S IF NEED SOME??? 
        
        # Prologue
        addiu $sp $sp -20 
        sw $ra 0($sp)
        sw $a0 4($sp)
        sw $a1 8($sp)
        sw $a2 12($sp)
        sw $s0 16($sp)
        
        # Create new node. 
        jal new_node
        
        # So now $v0 contains the pointer to new node. 
        # Move new node pointer into $s0. 
	addu $s0 $v0 $0 
	
	# Now copy string - call the fxn. 
	# Don't forget put string to copy into $a0.
	lw $a1 8($sp) 
	addu $a0 $a1 $0 
	jal copy_of_str
	
	# So now $v0 should contain pointer to string copy. 
	
	# Now just stick stuff into node, and put to front, and return. 
	
	# Stick stuff into node. 
	sw $v0 0($s0) # Name 
	lw $a2 12($sp) 
	sw $a2 4($s0) # Addr
	lw $a0 4($sp) 
	sw $a0 8($s0) # Next
	
	# Return the new node front. 
	addu $v0 $s0 $0 
	
	
	# EPILOGUE
	lw $ra 0($sp)
	lw $a0 4($sp) 
	lw $a1 8($sp)
	lw $a2 12($sp)
	lw $s0 16($sp)
	addiu $sp $sp 20 
     
        jr $ra
        
        # End fxn add_to_list
        

	

###############################################################################
#                 DO NOT MODIFY ANYTHING BELOW THIS POINT                       
###############################################################################

#------------------------------------------------------------------------------
# function symbol_for_addr()
#------------------------------------------------------------------------------
# Iterates through the SymbolList and searches for an entry with the given addr.
# If an entry is found, return a pointer to the name. Otherwise return NULL.
#
# Arguments:
#  $a0 = pointer to a SymbolList (NULL indicates empty list)
#  $a1 = addr to look for
#
# Returns: a pointer to the name if found or NULL if not found
#------------------------------------------------------------------------------
symbol_for_addr:
        beq $a0, $0, symbol_not_found   # Begin symbol_for_addr
        lw $t0, 4($a0)
        beq $t0, $a1, symbol_found
        lw $a0, 8($a0)
        j symbol_for_addr
symbol_found:
        lw $v0, 0($a0)
        jr $ra
symbol_not_found:
        li $v0, 0
        jr $ra                  # End addr_for_symbol

#------------------------------------------------------------------------------
# function print_list() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Arguments:
#  $a0 = pointer to a SymbolList (NULL indicates empty list)
#  $a1 = print function
#  $a2 = file pointer
#------------------------------------------------------------------------------
print_list:
        addiu $sp, $sp, -16             # Begin print_list
        sw $s0, 12($sp)
        sw $s1, 8($sp)
        sw $s2, 4($sp)
        sw $ra, 0($sp)
        move $s0, $a0
        move $s1, $a1
        move $s2, $a2
print_list_loop:
        beq $s0, $0, print_list_end
        lw $a0, 4($s0)
        lw $a1, 0($s0)
        move $a2, $s2
        jalr $s1
        lw $s0, 8($s0)
        j print_list_loop
print_list_end:
        lw $s0, 12($sp)
        lw $s1, 8($sp)
        lw $s2, 4($sp)
        lw $ra, 0($sp)
        addiu $sp, $sp, 16
        jr $ra                  # End print_list        

#------------------------------------------------------------------------------
# function print_symbol() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Prints one symbol to standard output.
#
# Arguments:
#  $a0 = addr of symbol
#  $a1 = name of symbol
#  $a2 = file pointer (this argument is actually ignored)
#
# Returns: none
#------------------------------------------------------------------------------
print_symbol:
        li $v0, 36                      # Begin print_symbol()
        syscall
        la $a0, tab
        li $v0, 4
        syscall
        move $a0, $a1
        syscall
        la $a0, newline
        syscall
        jr $ra                  # End print_symbol()

#------------------------------------------------------------------------------
# function write_symbol() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Writes one symbol to the file specified
#
# Arguments:
#  $a0 = addr of symbol
#  $a1 = name of symbol
#  $a2 = file pointer
#
# Returns: none
#------------------------------------------------------------------------------
write_symbol:           
        addiu $sp, $sp, -20             # Begin write_symbol()
        sw $s0, 16($sp)
        sw $s1, 12($sp)
        sw $a1, 8($sp)
        sw $a2, 4($sp)
        sw $ra, 0($sp)
        
        la $s0, temp_buf
        
        move $a1, $s0
        jal dec_to_str  # write int
        move $s1, $v0   
        
        addu $a0, $s0, $s1
        addiu $s1, $s1, 1
        la $a1, tab
        li $a2, 1
        jal strncpy             # write tab
        
        lw $a0, 8($sp)
        jal strlen
        
        addu $a0, $s0, $s1
        addu $s1, $s1, $v0
        lw $a1, 8($sp)
        move $a2, $v0
        jal strncpy             # write string
        
        addu $a0, $s0, $s1
        addiu $s1, $s1, 1
        la $a1, newline
        li $a2, 1
        jal strncpy             # write newline
        
        lw $a0, 4($sp)  # file ptr
        move $a1, $s0   # buffer to write
        move $a2, $s1   # num chars to write
        li $v0, 15
        syscall         # write to file
        
        lw $s0, 16($sp)
        lw $s1, 12($sp)
        lw $ra, 0($sp)
        addiu $sp, $sp, 20
        jr $ra                  # End write_symbol()
        
#------------------------------------------------------------------------------
# function new_node() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Creates a new uninitialized SymbolList node.
# Arguments: none
# Returns: pointer to a SymbolList node
#------------------------------------------------------------------------------
new_node:       
        li $a0, 12                      # Begin new_node()
        li $v0, 9
        syscall
        jr $ra                  # End new_node()
        
.data 
temp_buf:       .space 1024
