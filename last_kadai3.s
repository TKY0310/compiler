#
	#
	INITIAL_GP = 0x10008000
	INITIAL_SP = 0x7ffffffc
	# system call service number
	snp_service = 99

	.text
init:
	# initialize $gp (global pointer) and $sp (stack pointer)
	la $gp, INITIAL_GP
	la $sp, INITIAL_SP
	jal main # jump to 'main'
	nop
	li $v0, snp_service
	syscall
	nop
	# not reach here
stop:
	j stop
	nop

	.text 0x00001000
main:
	li $v0,0
	la $t0,RESULT
	sw $v0,0($t0)
	li $v0,0
	la $t0,RESULT
	sw $v0,4($t0)
	li $v0,0
	la $t0,RESULT
	sw $v0,8($t0)
	li $v0,0
	la $t0,RESULT
	sw $v0,12($t0)
	li $v0,1
	la $t0,RESULT
	sw $v0,16($t0)
LOOP1_START:
	la $t0,RESULT
	lw $v0,16($t0)
	nop
	addi $sp,$sp,4
	sw $v0,0($sp)
	li $v0,31
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	slt $t2,$v0,$t1
	beq $t2,$zero,LOOP1_END
	nop
IF1_START:
	la $t0,RESULT
	lw $v0,16($t0)
	nop
	addi $sp,$sp,-4
	sw $v0,0($sp)
	li $v0,15
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	div $v0,$t1
	mflo $v0
	addi $sp,$sp,-4
	sw $v0,0($sp)
	li $v0,15
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	mult $v0,$t1
	mflo $v0
	addi $sp,$sp,-4
	sw $v0,0($sp)
	la $t0,RESULT
	lw $v0,16($t0)
	nop
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	bne $t1,$v0,ELSE1
	nop
	la $t0,RESULT
	lw $v0,8($t0)
	nop
	addi $sp,$sp,-4
	sw $v0,0($sp)
	li $v0,1
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	add $v0,$v0,$t1
	la $t0,RESULT
	sw $v0,8($t0)
	j IF1_END
	nop
ELSE1:
IF2_START:
	la $t0,RESULT
	lw $v0,16($t0)
	nop
	addi $sp,$sp,-4
	sw $v0,0($sp)
	li $v0,3
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	div $v0,$t1
	mflo $v0
	addi $sp,$sp,-4
	sw $v0,0($sp)
	li $v0,3
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	mult $v0,$t1
	mflo $v0
	addi $sp,$sp,-4
	sw $v0,0($sp)
	la $t0,RESULT
	lw $v0,16($t0)
	nop
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	bne $t1,$v0,ELSE2
	nop
	la $t0,RESULT
	lw $v0,0($t0)
	nop
	addi $sp,$sp,-4
	sw $v0,0($sp)
	li $v0,1
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	add $v0,$v0,$t1
	la $t0,RESULT
	sw $v0,0($t0)
	j IF2_END
	nop
ELSE2:
IF3_START:
	la $t0,RESULT
	lw $v0,16($t0)
	nop
	addi $sp,$sp,-4
	sw $v0,0($sp)
	li $v0,5
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	div $v0,$t1
	mflo $v0
	addi $sp,$sp,-4
	sw $v0,0($sp)
	li $v0,5
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	mult $v0,$t1
	mflo $v0
	addi $sp,$sp,-4
	sw $v0,0($sp)
	la $t0,RESULT
	lw $v0,16($t0)
	nop
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	bne $t1,$v0,ELSE3
	nop
	la $t0,RESULT
	lw $v0,4($t0)
	nop
	addi $sp,$sp,-4
	sw $v0,0($sp)
	li $v0,1
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	add $v0,$v0,$t1
	la $t0,RESULT
	sw $v0,4($t0)
	j IF3_END
	nop
ELSE3:
	la $t0,RESULT
	lw $v0,12($t0)
	nop
	addi $sp,$sp,-4
	sw $v0,0($sp)
	li $v0,1
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	add $v0,$v0,$t1
	la $t0,RESULT
	sw $v0,12($t0)
IF3_END:
IF2_END:
IF1_END:
	la $t0,RESULT
	lw $v0,16($t0)
	nop
	addi $sp,$sp,-4
	sw $v0,0($sp)
	li $v0,1
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	add $v0,$v0,$t1
	la $t0,RESULT
	sw $v0,16($t0)
	j LOOP1_START
	nop
LOOP1_END:
	jr $ra
	nop

	#
	#data segment
	#
	.data 0x10004000
RESULT:
	.word 0x0000 #fizz
	.word 0x0000 #buzz
	.word 0x0000 #fizzbuzz
	.word 0x0000 #others
	.word 0x0000 #i
