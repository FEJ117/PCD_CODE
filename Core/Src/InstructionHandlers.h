#ifndef INSTRUCTIONHANDLERS_H
#define	INSTRUCTIONHANDLERS_H

#include "Instruction.h"

typedef enum{
REG_NUMBER = 0,
INT_NUMBER = 1,
OTHER_DATA = 2
}Instruction_DataType_t;

void InstructionHandlers_INIT();

void InstructionHandlers_ProcessData(Instruction *exe);

//Add your own here

void op_EMP_BEG_END(Instruction *exe);
    


void op_PIC(Instruction *exe);
    


void op_SET(Instruction *exe);
    


void op_INC_DEC(Instruction *exe);
    


void op_COP(Instruction *exe);
    


void op_ADD_SUB(Instruction *exe);
    


void op_SMA_BIG(Instruction *exe);
    


void op_REQ_RNQ(Instruction *exe);



void op_VEQ_VNQ(Instruction *exe);



void op_ANH_ANL(Instruction *exe);
    


void op_SVA(Instruction *exe);
    


void op_INH_INL(Instruction *exe);
    


void op_TON(Instruction *exe);
    


void op_PTR(Instruction *exe);
    


void op_PCH(Instruction *exe);
    


void op_CLR(Instruction *exe);
    


void op_WAI(Instruction *exe);
    


void op_SPO(Instruction *exe);
    


void op_JPO(Instruction *exe);
    


void op_JUM(Instruction *exe);
    


void op_LD1_LD2(Instruction *exe);
    



#endif