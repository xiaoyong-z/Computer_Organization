.data
passstring:     .asciiz "all tests passed!\n"
fail1string:    .asciiz "test #"
fail2string:    .asciiz " failed!\n"

.text

# _start is the entry point into any program.

back2:
        j pass2
back4:
        j pass4

backjr:
        j passjr

backjal:
        li $4, 0xdeadbeef
        jr $31

backjalr:
        li $4, 0xdeadbeef
        jr $21

.global _start
.ent    _start
_start:

#
#  The header ends here, and code goes below
#

        # test #1: beq forwards
        ori   $30, 1
        ori   $2, $0, 1
        ori   $3, $0, 1
        beq   $2, $3, pass1
        j     fail
pass1:
        # test #2: beq backwards
        ori   $30, 2
        ori   $2, $0, 1
        ori   $3, $0, 1
        beq   $2, $3, back2
        j     fail
pass2:
        # test #3: bne forwards
        ori   $30, 3
        ori   $2, $0, 1
        ori   $3, $0, 1
        bne   $2, $3, fail3
        j     pass3
fail3:
        j     fail
pass3:
        # test #4: bne backwards
        ori   $30, 4
        ori   $2, $0, 1
        ori   $3, $0, 2
        bne   $2, $3, back4
        j     fail
pass4:
        # test #5: sll
        ori   $30, $0, 5
        ori   $3, $0, 1024
        sll   $4, $3, 2
        ori   $5, $0, 4096
        bne   $4, $5, fail

        # test #6: lui
        ori   $30, $0, 6
        ori   $3, $0, 0xdead
        sll   $3, $3, 16
        lui   $4, 0xdead
        bne   $4, $3, fail

        # test #7: srl positive
        ori   $30, $0, 7
        ori   $3, $0, 1024
        srl   $4, $3, 10
        ori   $5, $0, 1
        bne   $4, $5, fail

        # test #8: srl negative
        ori   $30, $0, 8
        li    $3, 0xffffffff
        srl   $4, $3, 16
        ori   $5, $0, 0xffff
        bne   $4, $5, fail

        # test #9: sra positive
        ori   $30, $0, 9
        ori   $3, $0, 1024
        srl   $4, $3, 1
        ori   $5, $0, 512
        bne   $4, $5, fail

        # test #10: sra negative
        ori   $30, $0, 10
        li    $3, 0xfffffff6
        sra   $4, $3, 1       # $4: 0xfffffffb
        li    $5, 0xfffffffb
        bne   $4, $5, fail

        # test #11: addu
        ori   $30, $0, 11
        li    $2, 1000
        li    $3, 337
        addu  $4, $2, $3
        li    $5, 1337
        bne   $4, $5, fail

        # test #12: addu overflow
        ori   $30, $0, 12
        li    $2, 0x7fffffff
        li    $3, 0x00000001
        addu  $4, $2, $3
        li    $5, 0x80000000
        bne   $4, $5, fail

        # test #13: subu
        ori   $30, $0, 13
        li    $2, -3
        li    $3, -1340
        subu  $4, $2, $3
        li    $5, 1337
        bne   $4, $5, fail

        # test #14: subu underflow
        ori   $30, $0, 14
        li    $2, 0x80000000
        li    $3, 0x00000001
        subu  $4, $2, $3
        li    $5, 0x7fffffff
        bne   $4, $5, fail

        # test #15: and
        ori   $30, $0, 15
        li    $2, 0x8fc023bd
        li    $3, 0x29e9a358
        and   $4, $2, $3
        li    $5, 0x09c02318
        bne   $4, $5, fail

        # test #16: or
        ori   $30, $0, 16
        li    $2, 0x8fc023bd
        li    $3, 0x29e9a358
        or    $4, $2, $3
        li    $5, 0xafe9a3fd
        bne   $4, $5, fail

        # test #17: xor
        ori   $30, $0, 17
        li    $2, 0x8fc023bd
        li    $3, 0x29e9a358
        xor   $4, $2, $3
        li    $5, 0xa62980e5
        bne   $4, $5, fail

        # test #18: nor
        ori   $30, $0, 18
        li    $2, 0x8fc023bd
        li    $3, 0x29e9a358
        nor   $4, $2, $3
        li    $5, 0x50165c02
        bne   $4, $5, fail

        # test #19: slt - < +
        ori   $30, $0, 19
        li    $2, 0xffffffff
        li    $3, 0x0000000f
        slt   $4, $2, $3
        li    $5, 1
        bne   $4, $5, fail

        # test #20: slt + > -
        ori   $30, $0, 20
        li    $2, 0x0000000f
        li    $3, 0xffffffff
        slt   $4, $2, $3
        li    $5, 0
        bne   $4, $5, fail

        # test #21: slt - < -
        ori   $30, $0, 21
        li    $2, 0xfffffff0
        li    $3, 0xffffffff
        slt   $4, $2, $3
        li    $5, 1
        bne   $4, $5, fail

        # test #22: slt - > -
        ori   $30, $0, 22
        li    $2, 0xffffffff
        li    $3, 0xfffffff0
        slt   $4, $2, $3
        li    $5, 0
        bne   $4, $5, fail

        # test #23: slt + < +
        ori   $30, $0, 23
        li    $2, 0x00000001
        li    $3, 0x0000000f
        slt   $4, $2, $3
        li    $5, 1
        bne   $4, $5, fail

        # test #24: slt + > +
        ori   $30, $0, 24
        li    $2, 0x0000000f
        li    $3, 0x00000001
        slt   $4, $2, $3
        li    $5, 0
        bne   $4, $5, fail

        # test #25: sltu normal less
        ori   $30, $0, 25
        li    $2, 0x00000003
        li    $3, 0x0000000f
        sltu  $4, $2, $3
        li    $5, 1
        bne   $4, $5, fail

        # test #26: sltu normal greater
        ori   $30, $0, 26
        li    $2, 0x0000000f
        li    $3, 0x00000003
        sltu  $4, $2, $3
        li    $5, 0
        bne   $4, $5, fail

        # test #27: sltu signed negative less
        ori   $30, $0, 27
        li    $2, 0x00000001  # signed 1
        li    $3, 0xffffffff  # signed -1
        sltu  $4, $2, $3
        li    $5, 1
        bne   $4, $5, fail

        # test #28: sltu signed negative greater
        ori   $30, $0, 28
        li    $2, 0xffffffff  # signed -1
        li    $3, 0x00000001  # signed 1
        sltu  $4, $2, $3
        li    $5, 0
        bne   $4, $5, fail

        # test #29: mflo and mult
        ori   $30, $0, 29
        li    $2, 0x00000042
        li    $3, 0x00001337
        mult  $2, $3
        mflo  $4
        li    $5, 0x0004f42e
        bne   $4, $5, fail

        # test #30: mfhi
        ori   $30, $0, 30
        li    $2, 0x7fffffff
        li    $3, 0x00000010
        mult  $2, $3
        mfhi  $4
        li    $5, 0x00000007
        bne   $4, $5, fail

        # test #31: mult - * - lo
        ori   $30, $0, 31
        li    $2, -5
        li    $3, -6
        mult  $2, $3
        mflo  $4
        li    $5, 30
        bne   $4, $5, fail

        # test #32: mult - * - hi
        ori   $30, $0, 32
        li    $2, -5
        li    $3, -6
        mult  $2, $3
        mfhi  $4
        li    $5, 0
        bne   $4, $5, fail

        # test #33: mult - * + lo
        ori   $30, $0, 33
        li    $2, -5
        li    $3, 6
        mult  $2, $3
        mflo  $4
        li    $5, 0xffffffe2
        bne   $4, $5, fail

        # test #34: mult - * + hi
        ori   $30, $0, 34
        li    $2, -5
        li    $3, 6
        mult  $2, $3
        mfhi  $4
        li    $5, 0xffffffff
        bne   $4, $5, fail

        # test #35: multu lo
        ori   $30, $0, 35
        li    $2, 0xffffffff
        li    $3, 0x00000100
        multu $2, $3
        mflo  $4
        li    $5, 0xffffff00
        bne   $4, $5, fail

        # test #36: multu hi
        ori   $30, $0, 36
        li    $2, 0xffffffff
        li    $3, 0x00000100
        multu $2, $3
        mfhi  $4
        li    $5, 0x000000ff
        bne   $4, $5, fail

        # test #37: addiu
        ori   $30, $0, 37
        li    $2, 1000
        addiu  $4, $2, 337
        li    $5, 1337
        bne   $4, $5, fail

        # test #38: addiu ext
        ori   $30, $0, 38
        addiu $4, $0, -1337
        li    $5, -1337
        bne   $4, $5, fail

        # test #39: addiu overflow
        ori   $30, $0, 39
        li    $2, 0x7fffffff
        addiu $4, $2, 1
        li    $5, 0x80000000
        bne   $4, $5, fail

        # test #40: addiu underflow
        ori   $30, $0, 40
        li    $2, 0x80000000
        addiu $4, $2, -1
        li    $5, 0x7fffffff
        bne   $4, $5, fail

        # test #41: slti - < +
        ori   $30, $0, 41
        li    $2, 0xffffffff
        slti  $4, $2, 15
        li    $5, 1
        bne   $4, $5, fail

        # test #42: slti + > -
        ori   $30, $0, 42
        li    $2, 0x0000000f
        slti  $4, $2, -1
        li    $5, 0
        bne   $4, $5, fail

        # test #43: slti - < -
        ori   $30, $0, 43
        li    $2, 0xfffffff0
        slti  $4, $2, -1
        li    $5, 1
        bne   $4, $5, fail

        # test #44: slti - > -
        ori   $30, $0, 44
        li    $2, 0xffffffff
        slti  $4, $2, -2
        li    $5, 0
        bne   $4, $5, fail

        # test #45: slti + < +
        ori   $30, $0, 45
        li    $2, 0x00000001
        slti  $4, $2, 15
        li    $5, 1
        bne   $4, $5, fail

        # test #46: slti + > +
        ori   $30, $0, 46
        li    $2, 0x0000000f
        slti  $4, $2, 1
        li    $5, 0
        bne   $4, $5, fail

        # test #47: sltiu normal less
        ori   $30, $0, 47
        li    $2, 0x00000003
        sltiu $4, $2, 15
        li    $5, 1
        bne   $4, $5, fail

        # test #48: sltiu normal greater
        ori   $30, $0, 48
        li    $2, 0x0000000f
        sltiu $4, $2, 3
        li    $5, 0
        bne   $4, $5, fail

        # test #49: sltiu signed negative less
        ori   $30, $0, 49
        li    $2, 0x00000001  # signed 1
        sltiu $4, $2, -1
        li    $5, 1
        bne   $4, $5, fail

        # test #50: sltiu signed negative greater
        ori   $30, $0, 50
        li    $2, 0xffffffff  # signed -1
        sltiu $4, $2, 1
        li    $5, 0
        bne   $4, $5, fail

        # test #51: andi
        ori   $30, $0, 51
        li    $2, 0x8fc023bd
        andi  $4, $2, 0xa358
        li    $5, 0x00002318
        bne   $4, $5, fail

        # test #52: xori
        ori   $30, $0, 52
        li    $2, 0x8fc023bd
        xori  $4, $2, 0xa358
        li    $5, 0x8fc080e5
        bne   $4, $5, fail

        # test #53: lb
        ori   $30, $0, 53
        la    $3, one
        lb    $4, 0($3)
        li    $5, 0x78
        bne   $4, $5, fail

        # test #54: lb offset
        ori   $30, $0, 54
        la    $3, one
        lb    $4, 3($3)
        li    $5, 0x12
        bne   $4, $5, fail

        # test #55: lb offset 2
        ori   $30, $0, 55
        la    $3, zero
        lb    $4, 5($3)
        li    $5, 0x56
        bne   $4, $5, fail

        # test #56: lb negative offset
        ori   $30, $0, 56
        la    $3, two
        lb    $4, -2($3)
        li    $5, 0x34
        bne   $4, $5, fail

        # test #57: lb sign extension
        ori   $30, $0, 57
        la    $3, two
        lb    $4, 0($3)
        li    $5, 0xffffffff
        bne   $4, $5, fail

        # test #58: lh
        ori   $30, $0, 58
        la    $3, one
        lh    $4, 0($3)
        li    $5, 0x5678
        bne   $4, $5, fail

        # test #59: lh offset
        ori   $30, $0, 59
        la    $3, one
        lh    $4, 2($3)
        li    $5, 0x1234
        bne   $4, $5, fail

        # test #60: lh offset 2
        ori   $30, $0, 60
        la    $3, zero
        lh    $4, 6($3)
        li    $5, 0x1234
        bne   $4, $5, fail

        # test #61: lh negative offset
        ori   $30, $0, 61
        la    $3, two
        lh    $4, -4($3)
        li    $5, 0x5678
        bne   $4, $5, fail

        # test #62: lh sign extension
        ori   $30, $0, 62
        la    $3, two
        lh    $4, 0($3)
        li    $5, 0xffffffff
        bne   $4, $5, fail

        # test #63: lw
        ori   $30, $0, 63
        la    $3, one
        lw    $4, 0($3)
        li    $5, 0x12345678
        bne   $4, $5, fail

        # test #64: lw offset
        ori   $30, $0, 64
        la    $3, zero
        lw    $4, 8($3)
        li    $5, 0xffffffff
        bne   $4, $5, fail

        # test #65: lw negative offset
        ori   $30, $0, 65
        la    $3, four
        lw    $4, -12($3)
        li    $5, 0x12345678
        bne   $4, $5, fail

        # test #66: lbu offset
        ori   $30, $0, 66
        la    $3, one
        lbu   $4, 3($3)
        li    $5, 0x12
        bne   $4, $5, fail

        # test #67: lbu offset 2
        ori   $30, $0, 67
        la    $3, zero
        lbu   $4, 5($3)
        li    $5, 0x56
        bne   $4, $5, fail

        # test #68: lbu negative offset
        ori   $30, $0, 68
        la    $3, two
        lbu   $4, -2($3)
        li    $5, 0x34
        bne   $4, $5, fail

        # test #69: lbu zero extension
        ori   $30, $0, 69
        la    $3, two
        lbu   $4, 0($3)
        li    $5, 0x000000ff
        bne   $4, $5, fail

        # test #70: lhu
        ori   $30, $0, 70
        la    $3, one
        lhu   $4, 0($3)
        li    $5, 0x5678
        bne   $4, $5, fail

        # test #71: lhu offset
        ori   $30, $0, 71
        la    $3, one
        lhu   $4, 2($3)
        li    $5, 0x1234
        bne   $4, $5, fail

        # test #72: lhu offset 2
        ori   $30, $0, 72
        la    $3, zero
        lhu   $4, 6($3)
        li    $5, 0x1234
        bne   $4, $5, fail

        # test #73: lhu negative offset
        ori   $30, $0, 73
        la    $3, two
        lhu   $4, -4($3)
        li    $5, 0x5678
        bne   $4, $5, fail

        # test #74: lhu zero extension
        ori   $30, $0, 74
        la    $3, two
        lhu   $4, 0($3)
        li    $5, 0x0000ffff
        bne   $4, $5, fail

        # test #75: sw
        ori   $30, $0, 75
        la    $3, six
        li    $4, 0x87654321
        sw    $4, 0($3)
        lw    $5, 0($3)
        bne   $4, $5, fail
        sw    $0, 0($3)

        # test #76: sw offset
        ori   $30, $0, 76
        la    $3, six
        li    $4, 0x87654321
        sw    $4, 8($3)
        lw    $5, 8($3)
        bne   $4, $5, fail
        sw    $0, 8($3)

        # test #77: sw negative offset
        ori   $30, $0, 77
        la    $3, six
        li    $4, 0x87654321
        sw    $4, -4($3)
        lw    $5, -4($3)
        bne   $4, $5, fail
        sw    $0, -4($3)

        # test #78: sh
        ori   $30, $0, 78
        la    $3, six
        li    $2, 0x87654321
        sh    $2, 0($3)
        li    $4, 0x4321
        lhu   $5, 0($3)
        bne   $4, $5, fail
        sh    $0, 0($3)

        # test #79: sh offset
        ori   $30, $0, 79
        la    $3, six
        li    $2, 0x87654321
        sh    $2, 6($3)
        li    $4, 0x4321
        lhu   $5, 6($3)
        bne   $4, $5, fail
        sh    $0, 6($3)

        # test #80: sh negative offset
        ori   $30, $0, 80
        la    $3, six
        li    $2, 0x87654321
        sh    $2, -2($3)
        li    $4, 0x4321
        lhu   $5, -2($3)
        bne   $4, $5, fail
        sh    $0, -2($3)

        # test #81: sb
        ori   $30, $0, 81
        la    $3, six
        li    $2, 0x87654321
        sb    $2, 0($3)
        li    $4, 0x21
        lbu   $5, 0($3)
        bne   $4, $5, fail
        sb    $0, 0($3)

        # test #82: sb offset
        ori   $30, $0, 82
        la    $3, six
        li    $2, 0x87654321
        sb    $2, 7($3)
        li    $4, 0x21
        lbu   $5, 7($3)
        bne   $4, $5, fail
        sb    $0, 7($3)

        # test #83: sb negative offset
        ori   $30, $0, 83
        la    $3, six
        li    $2, 0x87654321
        sb    $2, -3($3)
        li    $4, 0x21
        lbu   $5, -3($3)
        bne   $4, $5, fail
        sb    $0, -3($3)

        # test #84: jr
        ori   $30, $0, 84
        la    $3, backjr
        jr    $3
        j     fail
passjr:

        # test #85: jal
        ori   $30, $0, 85
        li    $4, 0
        jal   backjal
        li    $5, 0xdeadbeef
        bne   $4, $5, fail

        # test #86: jalr
        ori   $30, $0, 86
        la    $3, backjalr
        li    $4, 0
        jalr  $21, $3
        li    $5, 0xdeadbeef
        bne   $4, $5, fail

pass:
        la $a0, passstring
        li $v0, 4
        syscall
        b done

fail:
        la $a0, fail1string
        ori $v0, $0, 4
        syscall

        ori  $a0, $30, 0
        ori  $v0, $0, 1
        syscall

        la $a0, fail2string
        ori $v0, $0, 4
        syscall

done:
        ori $v0, $zero, 10
        syscall

.end _start

.data
zero:  .word 0x00000000
one:   .word 0x12345678
two:   .word 0xffffffff
three: .word 0x00000000
four:  .word 0x00000000
five:  .word 0x00000000
six:   .word 0x00000000
seven: .word 0x00000000
eight: .word 0x00000000
