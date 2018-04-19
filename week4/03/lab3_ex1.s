        .data
        .word 2, 4, 6, 8
n:      .word 9


        .text
main:		lb 	$t0, 10($t1)	
		li      $v0, 10		
		syscall			

