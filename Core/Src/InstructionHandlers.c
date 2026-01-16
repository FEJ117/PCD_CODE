#include "InstructionHandlers.h"
#include "Instruction.h"
#include <stdint.h>
#include "Display.h"
#include "EEPROM.h"
#include "InstructionList.h"
#include "STM_FUNCTIONS.h"

extern uint8_t regPointer;
extern uint8_t registers[];
Instruction_DataType_t currentDataType;
uint8_t currentData;

/**
 * @brief Position of the cursor for PCH and PTR commands
 */
uint8_t cursPos = 0;

extern uint16_t programIndex;

void InstructionHandlers_ProcessData(Instruction *exe)
{
    if(exe->data == 'R')
    {
        currentDataType = REG_NUMBER;
        if (exe->data3 >= 58 || exe->data3 <= 48)
        currentData = (exe->data2 - 48);
        else if (exe->data2 >= 58 || exe->data2 <= 48)
        currentData=((exe->data2 - 48) * 10 + exe->data3 - 48);
        else currentData = 0; //Throw Exception
    }
    else if (exe->data >= 48 && exe->data <= 58) {
        currentDataType = INT_NUMBER;
        if (exe->data3 >= 48 && exe->data3 <= 58) {
            currentData = ((exe->data - 48) * 100 + (exe->data2 - 48) * 10
                    + exe->data3 - 48);
        } else if (exe->data2 >= 48 && exe->data2 <= 58) {
            currentData = ((exe->data - 48) * 10 + exe->data2 - 48);
        } else{
            currentData = (exe->data - 48);
        }
    }
    else {
        currentDataType = TONE;
    }
}

/**
  * @brief 	Evaluates, if a condition is true
  * 		- If BEG and END are being used:
  * 			# If the condition is true: Jump into the block (one position after BEG)
  * 			# If the condition is false: Jump one position after END
* 			- If BEG and END are not being used:
* 				# If the condition is true: Jumps to the next instruction
* 				# If the contition is false: Jumps to the instruction after the next
  * @param condition The condition which is being processed
  */
static void InstructionList_EvaluateCondition(bool condition) {

	if(!condition)
	{
		uint8_t openedBrackets = 1;
		if (EEPROM_GetFunctionNumber(programIndex) == FUNCTION_BEG) {
			while (EEPROM_GetFunctionNumber(programIndex) != FUNCTION_END
					|| openedBrackets > 0) {
				programIndex++;
				if (EEPROM_GetFunctionNumber(programIndex) == FUNCTION_BEG)
					openedBrackets++;
				if (EEPROM_GetFunctionNumber(programIndex) == FUNCTION_END)
					openedBrackets--;
			}
		}
		programIndex++;
	}
}

void InstructionHandlers_INIT()
{
    for(uint8_t i = 0; i < 100; i++)
    {
    	registers[i] = 0;
    }
    programIndex = 0;
    regPointer = 0;
    cursPos = 0;

    lastWaitTick = HAL_GetTick();
}

/**
  * @brief Writes up to 3 characters at the current position of the cursor variable (cursPos)
  * @param str Characters to be written (character that equal 0 will be ignored)
  */
static void InstructionList_WriteAtCursor(char str[3])
{
	Display_WriteString(str, 3, cursPos, 0);
	for(uint8_t i = 0; i < 3; i++)
	{
		if(str[i] != 0)cursPos++;
	}
}

//Add your own here

void op_EMPTY(Instruction *exe)
{
    
}

void op_PIC(Instruction *exe)
{
    regPointer = currentData;
}

void op_SET(Instruction *exe)
{
    registers[regPointer] = currentData;
}

void op_INC(Instruction *exe)
{
    registers[regPointer] = registers[regPointer] + currentData;
}

void op_DEC(Instruction *exe)
{
    registers[regPointer] = registers[regPointer] - currentData;
}

void op_COP(Instruction *exe)
{
    registers[currentData] = registers[regPointer];
}

void op_ADD(Instruction *exe)
{
    registers[regPointer] += registers[currentData];
}

void op_SUB(Instruction *exe)
{
    registers[regPointer] -= registers[currentData];
}

void op_SMA(Instruction *exe)
{
    InstructionList_EvaluateCondition(registers[regPointer] < registers[currentData]);
}

void op_BIG(Instruction *exe)
{
    InstructionList_EvaluateCondition(registers[regPointer] > registers[currentData]);
}

void op_REQ(Instruction *exe)
{
    InstructionList_EvaluateCondition(registers[regPointer] == registers[currentData]);
}

void op_RNQ(Instruction *exe)
{
    InstructionList_EvaluateCondition(registers[regPointer] != registers[currentData]);
}

void op_VEQ(Instruction *exe)
{
    InstructionList_EvaluateCondition(currentData == registers[regPointer]);
}

void op_VNQ(Instruction *exe)
{
    InstructionList_EvaluateCondition(!(currentData == registers[regPointer]));
}

void op_ANH(Instruction *exe)
{
    InstructionList_EvaluateCondition(registers[regPointer] < STM_ReadADC(currentData));
}

void op_ANL(Instruction *exe)
{
    InstructionList_EvaluateCondition(!(registers[regPointer] < STM_ReadADC(currentData)));
}

void op_SVA(Instruction *exe)
{
    registers[regPointer] = STM_ReadADC(currentData);
}

void op_INH(Instruction *exe)
{
    InstructionList_EvaluateCondition(STM_IsInputHigh(currentData));
}

void op_INL(Instruction *exe)
{
    InstructionList_EvaluateCondition(!(STM_IsInputHigh(currentData)));
}

void op_TON(Instruction *exe)
{
    STM_ActivateBuzzer(*exe); 
}

void op_PTR(Instruction *exe)
{
    char str[3];
    STM_Number3ToChar(registers[currentData], str);
    InstructionList_WriteAtCursor(str);
}

void op_PCH(Instruction *exe)
{
    char str[] = {exe->data, exe->data2, exe->data3};
	InstructionList_WriteAtCursor(str);
}

void op_CLR(Instruction *exe)
{
    cursPos = 0;
	Display_FillBlack();
}

void op_BEG(Instruction *exe)
{
    
}

void op_END(Instruction *exe)
{
    
}

void op_WAI(Instruction *exe)
{
    STM_Wait(currentData);
}

void op_SPO(Instruction *exe)
{
    registers[currentData] = programIndex-1;
}

void op_JPO(Instruction *exe)
{
    programIndex = registers[currentData];
}

void op_JUM(Instruction *exe)
{
    programIndex = currentData;
}

void op_LD1(Instruction *exe)
{
    STM_SetLED(exe->functionNumber, exe->data);
}

void op_LD2(Instruction *exe)
{
    STM_SetLED(exe->functionNumber, exe->data);
}