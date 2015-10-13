# CS 61C Summer 2015 Project 2-2 
# string.s

#==============================================================================
#                              Project 2-2 Part 1
#                               String README
#==============================================================================
# In this file you will be implementing some utilities for manipulating strings.
# The functions you need to implement are:
#  - strlen()
#  - strncpy()
#  - copy_of_str()
# Test cases are in linker-tests/test_string.s
#==============================================================================

.data
newline:	.asciiz "\n"
tab:	.asciiz "\t"

.text
#------------------------------------------------------------------------------
# function strlen()
#------------------------------------------------------------------------------
# Arguments:
#  $a0 = string input
#
# Returns: the length of the string
#------------------------------------------------------------------------------
strlen:
	add $v0, $0, $0		# initialize length to 0

	count:
	lb $t0, 0($a0)		# load next char
	beqz $t0, exit		# exit if null character
	addiu $v0, $v0, 1	# increment length by 1
	addiu $a0, $a0, 1	# point to next char in string
	j count 			# loop again

	exit:
	jr $ra

#------------------------------------------------------------------------------
# function strncpy()
#------------------------------------------------------------------------------
# Arguments:
#  $a0 = pointer to destination array
#  $a1 = source string
#  $a2 = number of characters to copy
#
# Returns: the destination array
#------------------------------------------------------------------------------
strncpy:
	add $t0, $0, $0		# initialize counter to 0
	move $v0, $a0		# store pointer to dest array in $v0

	copy:
	lb $t1, 0($a1)		# load next char from src string
	beq $t0, $a2, finish	# exit if reached num of chars to copy
	beqz $t1, pad		# pad with 0s if end of src reached before num
	lb $t2, 0($a0) 		# load next address from dest array
	add $t2, $t1, $0	# copy char from src reg to dest reg
	sb $t2, 0($a0)		# store new char value in dest array
	addiu $a0, $a0, 1	# point to next char in dest
	addiu $a1, $a1, 1	# point to next char in src
	addiu $t0, $t0, 1	# increment counter by 1
	j copy 				# loop again

	pad:
	lb $t2, 0($a0)		# load next address in dest array
	move $t2, $0		# place null value in dest reg
	sb $t2, 0($a0)		# store null value in dest array
	addiu $a0, $a0, 1	# point to next char in dest
	addiu $t0, $t0, 1	# increment counter by 1
	beq $t0, $a2, finish	# exit if reached num of chars to copy

	finish:
	addiu $a0, $a0, 1
	lb $t2, 0($a0)		# load next address in dest array
	move $t2, $0		# store 0
	sb $t2, 0($a0)
	jr $ra

#------------------------------------------------------------------------------
# function copy_of_str()
#------------------------------------------------------------------------------
# Creates a copy of a string. You will need to use sbrk (syscall 9) to allocate
# space for the string. strlen() and strncpy() will be helpful for this function.
# In MARS, to malloc memory use the sbrk syscall (syscall 9). See help for details.
#
# Arguments:
#   $a0 = string to copy
#
# Returns: pointer to the copy of the string
#------------------------------------------------------------------------------
copy_of_str:

	addi $sp, $sp, -24	# move stack pointer to store saved register in stack
	sw $ra, 0($sp)
	sw $a0, 4($sp)
	sw $a1, 8($sp)
	sw $a2, 12($sp)
	sw $s1, 16($sp)
	sw $s2, 20($sp)
	

	move $s1, $a0		# store source string in $s1		
	jal strlen
	move $s2, $v0		# load string length into $s2

	li $v0, 9			# load syscall # and allocate space for  string	
	move $a0, $s2
	syscall


	move $a0, $v0		# store address of allocated memory in $a0 in prep for calling strncpy
	move $a1, $s1
	move $a2, $s2
	jal strncpy			# copies string into allocated memory and returns pointer to it

	lw $ra, 0($sp)		# load back return address reg and move stack pointer back
	lw $a0, 4($sp)
	lw $a1, 8($sp)
	lw $a2, 12($sp)
	lw $s1, 16($sp)
	lw $s2, 20($sp)
	addiu $sp, $sp, 24
	jr $ra

###############################################################################
#                 DO NOT MODIFY ANYTHING BELOW THIS POINT                       
###############################################################################

#------------------------------------------------------------------------------
# function streq() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Arguments:
#  $a0 = string 1
#  $a1 = string 2
#
# Returns: 0 if string 1 and string 2 are equal, -1 if they are not equal
#------------------------------------------------------------------------------
streq:
	beq $a0, $0, streq_false	# Begin streq()
	beq $a1, $0, streq_false
streq_loop:
	lb $t0, 0($a0)
	lb $t1, 0($a1)
	addiu $a0, $a0, 1
	addiu $a1, $a1, 1
	bne $t0, $t1, streq_false
	beq $t0, $0, streq_true
	j streq_loop
streq_true:
	li $v0, 0
	jr $ra
streq_false:
	li $v0, -1
	jr $ra			# End streq()

#------------------------------------------------------------------------------
# function dec_to_str() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Convert a number to its unsigned decimal integer string representation, eg.
# 35 => "35", 1024 => "1024". 
#
# Arguments:
#  $a0 = int to write
#  $a1 = character buffer to write into
#
# Returns: the number of digits written
#------------------------------------------------------------------------------
dec_to_str:
	li $t0, 10			# Begin dec_to_str()
	li $v0, 0
dec_to_str_largest_divisor:
	div $a0, $t0
	mflo $t1		# Quotient
	beq $t1, $0, dec_to_str_next
	mul $t0, $t0, 10
	j dec_to_str_largest_divisor
dec_to_str_next:
	mfhi $t2		# Remainder
dec_to_str_write:
	div $t0, $t0, 10	# Largest divisible amount
	div $t2, $t0
	mflo $t3		# extract digit to write
	addiu $t3, $t3, 48	# convert num -> ASCII
	sb $t3, 0($a1)
	addiu $a1, $a1, 1
	addiu $v0, $v0, 1
	mfhi $t2		# setup for next round
	bne $t2, $0, dec_to_str_write
	jr $ra			# End dec_to_str()
