/**
 * @file InstructionHandlers.c
 * @brief Implements all instruction handlers as well as helper functions
 */

#include "InstructionHandlers.h"
#include "Instruction.h"
#include <stdint.h>
#include "Display.h"
#include "EEPROM.h"
#include "InstructionList.h"
#include "STM_FUNCTIONS.h"

/**
 * @brief Defines a type that specifies the data format of an instruction.
*/
typedef enum{
REG_NUMBER = 0,
INT_NUMBER = 1,
OTHER_DATA = 2
}Instruction_DataType_t;

/**
 * @brief Points at the chosen register (used by PIC and other register commands)
 */
static uint8_t regPointer = 0;


/**
 * @brief Values of the 100 virtual storage registers
 */
static uint16_t registers[100];

/**
* @brief The data type determined by InstructionHandlers_ProcessData()
*/
static Instruction_DataType_t currentDataType;

/**
* @brief The data value processed by InstructionHandlers_ProcessData()
*/
static uint8_t currentData;

/**
 * @brief Position of the cursor for PCH and PTR commands
 */
static uint8_t cursPos = 0;

/**
* @brief The current program index provided by InstructionList.h
*/
extern uint16_t programIndex;

//Documented in .h
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
        currentDataType = OTHER_DATA;
    }
}

/**
  * @brief 	Evaluates, if a condition is true
  * 		- If BEG and END are being used:
  * 			- If the condition is true: Jump into the block (one position after BEG)
  * 			- If the condition is false: Jump one position after END
  *
  * 		- If BEG and END are not being used:
  * 			- If the condition is true: Jumps to the next instruction
  * 			- If the contition is false: Jumps to the instruction after the next
  * @param condition The condition which is being processed
  */
static void EvaluateCondition(bool condition) {

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

//Documented in .h
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
static void WriteAtCursor(char str[3])
{
	Display_WriteString(str, 3, cursPos, 0);
	for(uint8_t i = 0; i < 3; i++)
	{
		if(str[i] != 0)cursPos++;
	}
}



//Add your own here



//Documented in .h
void op_EMP_BEG_END(Instruction *exe)
{
    
}

//Documented in .h
void op_PIC(Instruction *exe)
{
    if(currentDataType != REG_NUMBER) Display_ShowErrorMessage(programIndex-1);
    regPointer = currentData;
}

//Documented in .h
void op_SET(Instruction *exe)
{
    if(currentDataType != INT_NUMBER) Display_ShowErrorMessage(programIndex-1);
    registers[regPointer] = currentData;
}

//Documented in .h
void op_INC_DEC(Instruction *exe)
{
    if(currentDataType != INT_NUMBER) Display_ShowErrorMessage(programIndex-1);
    registers[regPointer] = 
    (exe->functionNumber == FUNCTION_INC) ? 
    (registers[regPointer] + currentData) : 
    (registers[regPointer] - currentData);
}


//Documented in .h
void op_COP(Instruction *exe)
{
    if(currentDataType != REG_NUMBER) Display_ShowErrorMessage(programIndex-1);
    registers[currentData] = registers[regPointer];
}

//Documented in .h
void op_ADD_SUB(Instruction *exe)
{
    if(currentDataType != REG_NUMBER) Display_ShowErrorMessage(programIndex-1);
    registers[regPointer] += 
    (exe->functionNumber == FUNCTION_ADD) ? 
    (registers[currentData]) :
    (-registers[currentData]);
}

//Documented in .h
void op_SMA_BIG(Instruction *exe)
{
    if(currentDataType != REG_NUMBER) Display_ShowErrorMessage(programIndex-1);
    EvaluateCondition(
        (exe->functionNumber == FUNCTION_SMA) ? 
        (registers[regPointer] < registers[currentData]) :
        (registers[regPointer] > registers[currentData])
    );
}

//Documented in .h
void op_REQ_RNQ(Instruction *exe)
{
    if(currentDataType != REG_NUMBER) Display_ShowErrorMessage(programIndex-1);
    EvaluateCondition(
        (exe->functionNumber == FUNCTION_REQ) ? 
        (registers[regPointer] == registers[currentData]) :
        (registers[regPointer] != registers[currentData])
    );
}

//Documented in .h
void op_VEQ_VNQ(Instruction *exe)
{
    if(currentDataType != INT_NUMBER) Display_ShowErrorMessage(programIndex-1);
    EvaluateCondition(
        (exe->functionNumber == FUNCTION_VEQ) ? 
        (currentData == registers[regPointer]) :
        (currentData != registers[regPointer])
    );
}

//Documented in .h
void op_ANH_ANL(Instruction *exe)
{
    if(currentDataType != REG_NUMBER) Display_ShowErrorMessage(programIndex-1);
    EvaluateCondition(
        (exe->functionNumber == FUNCTION_ANH) ? 
        (registers[regPointer] < STM_ReadADC(currentData)):
        (registers[regPointer] > STM_ReadADC(currentData))
    );
}

///Documented in .h
void op_SVA(Instruction *exe)
{
    if(currentDataType != INT_NUMBER) Display_ShowErrorMessage(programIndex-1);
    registers[regPointer] = STM_ReadADC(currentData);
}

//Documented in .h
void op_INH_INL(Instruction *exe)
{
    if(currentDataType != INT_NUMBER) Display_ShowErrorMessage(programIndex-1);
    EvaluateCondition(
        (exe->functionNumber == FUNCTION_INH) ?
        STM_IsInputHigh(currentData):
        STM_IsInputHigh(currentData)
    );
}

//Documented in .h
void op_TON(Instruction *exe)
{
    if(currentDataType != OTHER_DATA) Display_ShowErrorMessage(programIndex-1);
    STM_ActivateBuzzer(*exe); 
}

//Documented in .h
void op_PTR(Instruction *exe)
{
    if(currentDataType != REG_NUMBER) Display_ShowErrorMessage(programIndex-1);
    char str[3];
    STM_Number3ToChar(registers[currentData], str);
    WriteAtCursor(str);
}

//Documented in .h
void op_PCH(Instruction *exe)
{
    char str[] = {exe->data, exe->data2, exe->data3};
	WriteAtCursor(str);
}

//Documented in .h
void op_CLR(Instruction *exe)
{
    cursPos = 0;
	Display_FillBlack();
}

//Documented in .h
void op_WAI(Instruction *exe)
{
    if(currentDataType != INT_NUMBER) Display_ShowErrorMessage(programIndex-1);
    STM_Wait(currentData);
}

//Documented in .h
void op_SPO(Instruction *exe)
{
    if(currentDataType != REG_NUMBER) Display_ShowErrorMessage(programIndex-1);
    registers[currentData] = programIndex-1;
}

//Documented in .h
void op_JPO(Instruction *exe)
{
    if(currentDataType != REG_NUMBER) Display_ShowErrorMessage(programIndex-1);
    programIndex = registers[currentData];
}

//Documented in .h
void op_JUM(Instruction *exe)
{
    if(currentDataType != INT_NUMBER) Display_ShowErrorMessage(programIndex-1);
    programIndex = currentData;
}

//Documented in .h
void op_LD1_LD2(Instruction *exe)
{
    if(currentDataType != OTHER_DATA) Display_ShowErrorMessage(programIndex-1);
    STM_SetLED(exe->functionNumber, exe->data);
}