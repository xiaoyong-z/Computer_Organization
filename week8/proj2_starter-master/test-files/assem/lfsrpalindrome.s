	addi $a0 $0 0
	jal LfsrPalindrome	
LfsrPalindrome:	
	addi $t3 $0 1
	lfsr $t1 $a0
loop:
	beq $t1 $a0 end
	bitpal $t2 $t1
	beq $t2 $t3 end
	lfsr $t1 $t1
end:
	add $v0 $0 $t1
	jr $ra
	 
