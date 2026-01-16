#ifndef INSTRUCTIONHANDLERS_H
#define	INSTRUCTIONHANDLERS_H

#include "Instruction.h"

void InstructionHandlers_INIT();

void op_EMPTY(Instruction *exe);
    


void op_PIC(Instruction *exe);
    


void op_SET(Instruction *exe);
    


void op_INC(Instruction *exe);
    


void op_DEC(Instruction *exe);
    


void op_COP(Instruction *exe);
    


void op_ADD(Instruction *exe);
    


void op_SUB(Instruction *exe);
    


void op_SMA(Instruction *exe);
    


void op_BIG(Instruction *exe);
    


void op_REQ(Instruction *exe);
    


void op_RNQ(Instruction *exe);    


void op_VEQ(Instruction *exe);
    


void op_VNQ(Instruction *exe);
    


void op_AOU(Instruction *exe);
    


void op_DOU(Instruction *exe);
    


void op_ANH(Instruction *exe);
    


void op_ANL(Instruction *exe);
    


void op_SVA(Instruction *exe);
    


void op_INH(Instruction *exe);
    


void op_INL(Instruction *exe);
    


void op_TON(Instruction *exe);
    


void op_PTR(Instruction *exe);
    


void op_PCH(Instruction *exe);
    


void op_CLR(Instruction *exe);
    


void op_BEG(Instruction *exe);
    


void op_END(Instruction *exe);
    


void op_WAI(Instruction *exe);
    


void op_SPO(Instruction *exe);
    


void op_JPO(Instruction *exe);
    


void op_JUM(Instruction *exe);
    


void op_LD1(Instruction *exe);
    


void op_LD2(Instruction *exe);
    



#endif