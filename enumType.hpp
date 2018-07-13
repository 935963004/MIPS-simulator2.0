#ifndef ENUMTYPE_HPP
#define ENUMTYPE_HPP

enum REGISTER
{
    zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, 
    s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, gp, sp, fp, ra, hi, lo, pc, none
};

enum INSRTUCT
{
    Align, ascii, asciiz, byte, half, word, space, Data, text, add, addu, 
    addiu, sub, subu, mul, mulu, Div, divu, Xor, xoru, neg, negu, rem, remu, 
    seq, sge, sgt, sle, slt, sne, b, beq, bne, bge, ble, bgt, blt, beqz, bnez, 
    blez, bgez, bgtz, bltz, j, jr, jal, jalr, li, la, lb, lh, lw, sb, sh, sw, 
    Move, mfhi, mflo, nop, syscall
};

#endif