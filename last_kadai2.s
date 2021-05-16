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
	li $v0,1
	la $t0,RESULT
	sw $v0,4($t0)
	li $v0,1
	la $t0,RESULT
	sw $v0,0($t0)
LOOP1_START:
	la $t0,RESULT
	lw $v0,0($t0)
	nop
	addi $sp,$sp,4
	sw $v0,0($sp)
	li $v0,6
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	slt $t2,$v0,$t1
	beq $t2,$zero,LOOP1_END
	nop
	la $t0,RESULT
	lw $v0,4($t0)
	nop
	addi $sp,$sp,-4
	sw $v0,0($sp)
	la $t0,RESULT
	lw $v0,0($t0)
	nop
	add $t1,$v0,$zero
	lw $v0,0($sp)
	addi $sp,$sp,4
	mult $v0,$t1
	mflo $v0
	la $t0,RESULT
	sw $v0,4($t0)
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
	.word 0x0000 #i
	.word 0x0000 #fact
