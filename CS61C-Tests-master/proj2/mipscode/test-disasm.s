        sll	$1,$2,0
        sll	$3,$4,1
        srl	$5,$6,2
        sra	$7,$8,3
        jr	$9
        jalr	$10,$11
        syscall
        mfhi	$12
        mflo	$13
        mult	$14,$15
        multu	$16,$17
        addu	$18,$19,$20
        subu	$21,$22,$23
        and	$24,$25,$26
        or	$27,$28,$29
        xor	$30,$0,$31
        nor	$1,$2,$3
        slt	$1,$2,$3
a:      sltu	$1,$2,$3
b:      beq     $1,$2,a
c:      beq     $1,$2,f
d:      bne     $1,$2,d
e:      bne     $1,$2,g
f:      addiu	$1,$2,-41
g:      addiu	$1,$2,42
        slti	$1,$2,-41
        slti	$1,$2,42
        sltiu	$1,$2,-41
        sltiu	$1,$2,42
        andi	$1,$2,0xffff
        andi	$1,$2,0xdeaf
        andi	$1,$2,0xa
        andi	$1,$2,0xab
        andi	$1,$2,0xabc
        ori	$1,$2,0xffff
        ori	$1,$2,0xdeaf
        ori	$1,$2,0xa
        ori	$1,$2,0xab
        ori	$1,$2,0xabc
        xori	$1,$2,0xffff
        xori	$1,$2,0xdeaf
        xori	$1,$2,0xa
        xori	$1,$2,0xab
        xori	$1,$2,0xabc
        lui	$1,0xffff
        lui	$1,0xdeaf
        lui	$1,0xa
        lui	$1,0xab
        lui	$1,0xabc
        lb	$1,-1337($2)
        lb	$1,42($2)
        lh	$1,-1337($2)
        lh	$1,42($2)
        lw	$1,-1337($2)
        lw	$1,42($2)
        lbu	$1,-1337($2)
        lbu	$1,42($2)
        lhu	$1,-1337($2)
        lhu	$1,42($2)
        sb	$1,-1337($2)
        sb	$1,42($2)
        sh	$1,-1337($2)
        sh	$1,42($2)
        sw	$1,-1337($2)
        sw	$1,42($2)
