#include "InstructionHandlers.h"
#include "Instruction.h"
#include <stdint.h>
#include "Display.h"
#include "EEPROM.h"
#include "InstructionList.h"
#include "STM_FUNCTIONS.h"

/**
 * @brief Points at the chosen register (used by PIC and other register commands)
 */
uint8_t regPointer = 0;


/**
 * @brief Values of the 100 virtual storage registers
 */
uint16_t registers[100];

/**
* @brief The data type determined by InstructionHandlers_ProcessData()
*/
Instruction_DataType_t currentDataType;

/**
* @brief The data value processed by InstructionHandlers_ProcessData()
*/
uint8_t currentData;

/**
 * @brief Position of the cursor for PCH and PTR commands
 */
uint8_t cursPos = 0;

/**
* @brief The current program index provided by InstructionList.h
*/
extern uint16_t programIndex;

/**
* @brief Processes the data stored in the current instruction and sets the data type processed.
* @details The datatype can be used by the functions to determine if the data provided has the right format.
*/
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
  * 			# If the condition is true: Jump into the block (one position after BEG)
  * 			# If the condition is false: Jump one position after END
* 			- If BEG and END are not being used:
* 				# If the condition is true: Jumps to the next instruction
* 				# If the contition is false: Jumps to the instruction after the next
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

/**
* @brief Initializes the registers as well as the program index, cursor and register pointer to be 0.
* @warning Should be called everytime the program switches into executing mode
*/
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

/** @brief Handler for the instructions EMP, BEG and END. Does nothing. 
*/
void op_EMP_BEG_END(Instruction *exe)
{
    
}

/** @brief Handler for the instruction PIC. 
  * @details Sets the register pointer to the desired value specified in the data of the instruction.
 */
void op_PIC(Instruction *exe)
{
    regPointer = currentData;
}

/** @brief Handler for the instruction SET. 
  * @details Sets the value of the register pointed to by the register pointer
  * to the value specified in the data of the instruction.
 */
void op_SET(Instruction *exe)
{
    registers[regPointer] = currentData;
}

/** @brief Handler for the instructions INC and DEC. 
  * @details Increments or decrements the register pointed to by the register pointer
  * by the value specified in the data of the instruction.
 */
void op_INC_DEC(Instruction *exe)
{
    registers[regPointer] = 
    (exe->functionNumber == FUNCTION_INC) ? 
    (registers[regPointer] + currentData) : 
    (registers[regPointer] - currentData);
}


/** @brief Handler for the instruction COP. 
  * @details Copies the register pointed to by the register pointer into
  * the register specified by the data of the instruction.
 */
void op_COP(Instruction *exe)
{
    registers[currentData] = registers[regPointer];
}

/** @brief Handler for the instructions ADD and SUB. 
  * @details Adds or subtracts the value of the register specified by the data of the instruction from/to
  * the register pointed to by the register pointer.
 */
void op_ADD_SUB(Instruction *exe)
{
    registers[regPointer] += 
    (exe->functionNumber == FUNCTION_ADD) ? 
    (registers[currentData]) :
    (-registers[currentData]);
}

/** @brief Handler for the instructions SMA and BIG. 
  * @details Determines, if the value of the register pointed to by the register pointer is
  * smaller or bigger then the value of the register specified by the data of the instruction.
 */
void op_SMA_BIG(Instruction *exe)
{
    EvaluateCondition(
        (exe->functionNumber == FUNCTION_SMA) ? 
        (registers[regPointer] < registers[currentData]) :
        (registers[regPointer] > registers[currentData])
    );
}

/** @brief Handler for the instructions REQ and RNQ. 
  * @details Determines, if the values of the register pointed to by the register pointer and
  * the value of the register specified by the data of the instruction are equal or not equal.
 */
void op_REQ_RNQ(Instruction *exe)
{
    EvaluateCondition(
        (exe->functionNumber == FUNCTION_REQ) ? 
        (registers[regPointer] == registers[currentData]) :
        (registers[regPointer] != registers[currentData])
    );
}

/** @brief Handler for the instructions VEQ and VNQ. 
  * @details Determines, if the values of the register pointed to by the register pointer and
  * the value of the data of the instruction are equal or not equal.
 */
void op_VEQ_VNQ(Instruction *exe)
{
    EvaluateCondition(
        (exe->functionNumber == FUNCTION_VEQ) ? 
        (currentData == registers[regPointer]) :
        (currentData != registers[regPointer])
    );
}

/** @brief Handler for the instructions ANH and ANL. 
  * @details Determines, if the values of the register pointed to by the register pointer and
  * the value of the analog input provided by the data of the instuction are smaller or bigger.
  * @warning The value passed in the instruction is the ADC number of the analog input.
 */
void op_ANH_ANL(Instruction *exe)
{
    EvaluateCondition(
        (exe->functionNumber == FUNCTION_ANH) ? 
        (registers[regPointer] < STM_ReadADC(currentData)):
        (registers[regPointer] > STM_ReadADC(currentData))
    );
}

/** @brief Handler for the instructions REQ and RNQ. 
  * @details Determines, if the values of the register pointed to by the register pointer and
  * the value of the register specified by the data of the instruction are equal or not equal.
 */
void op_SVA(Instruction *exe)
{
    registers[regPointer] = STM_ReadADC(currentData);
}

/** @brief Handler for the instructions INH and INL. 
  * @details Determines, if the input specified by the data of the instruction is high or low.
  * @warning Input0 = Button1, Input1 = Button2, Input2 = Button3, Input3 = Button4
 */
void op_INH_INL(Instruction *exe)
{
    EvaluateCondition(
        (exe->functionNumber == FUNCTION_INH) ?
        STM_IsInputHigh(currentData):
        STM_IsInputHigh(currentData)
    );
}

/** @brief Handler for the instruction TON. 
  * @details Produces a speaker output representing the tone specified in the instruction's data.
  * @warning The tone must be provided in the following format: C#7, where C is any tone (C, D, E, F, G, A, H),
  * 7 is the pitch (choose from 1-7) and # indicates the semitone higher then C. # is optional.
 */
void op_TON(Instruction *exe)
{
    STM_ActivateBuzzer(*exe); 
}

/** @brief Handler for the instruction PTR. 
  * @details Prints the value of the register provided by the data of the instruction to the cursor position
  * and increments the cursor position.
 */
void op_PTR(Instruction *exe)
{
    char str[3];
    STM_Number3ToChar(registers[currentData], str);
    WriteAtCursor(str);
}

/** @brief Handler for the instruction PCH. 
  * @details Prints any three chars provided by the data of the instruction to the cursor position
  * and increments the cursor position. Chars which are not provided will be left out.
 */
void op_PCH(Instruction *exe)
{
    char str[] = {exe->data, exe->data2, exe->data3};
	WriteAtCursor(str);
}

/** @brief Handler for the instruction CLR. 
  * @details Clears the screen.
 */
void op_CLR(Instruction *exe)
{
    cursPos = 0;
	Display_FillBlack();
}

/** @brief Handler for the instruction WAI. 
  * @details Pauses the program for the amount of time given in the data of the instruction in 1/10 seconds.
 */
void op_WAI(Instruction *exe)
{
    STM_Wait(currentData);
}

/**
  * @brief Handler for the instruction SPO.
  * @details Saves the current position in the program to the register specified by the instruction's data.
*/
void op_SPO(Instruction *exe)
{
    registers[currentData] = programIndex-1;
}

/**
  * @brief Handler for the instruction JPO.
  * @details Jumps to the position pointed to by the register specified by the instruction's data.
*/
void op_JPO(Instruction *exe)
{
    programIndex = registers[currentData];
}

/**
  * @brief Handler for the instruction JUM.
  * @details Jumps to the position specified in the instruction's data.
*/
void op_JUM(Instruction *exe)
{
    programIndex = currentData;
}

/**
  * @brief Handler for the instruction LD1 and LD2.
  * @details Sets one of the LEDs (depending on LD1/LD2) to the colour specified by the instruction's data.
  * (R = Red, G = Green, B = Blue, O = Orange, T = Turquoise, V = Violett, W = White, Any other = OFF)
*/
void op_LD1_LD2(Instruction *exe)
{
    STM_SetLED(exe->functionNumber, exe->data);
}