/**
 * @file InstructionList.c
 * @brief Implementation of the main program logic for writing and executing commands
 */

#include "EEPROM.h"
#include "Instruction.h"
#include "PS2Driver.h"
#include "InstructionList.h"
#include "STM_FUNCTIONS.h"

//Documented in .h
uint16_t programIndex = 0;

/**
 * @brief Values of the 100 virtual storage registers
 */
uint16_t registers[100];

/**
 * @brief Points at the chosen register (used by PIC and other register commands)
 */
uint8_t regPointer = 0;

/**
 * @brief Position of the cursor for PCH and PTR commands
 */
uint8_t cursPos = 0;

/**
 * @brief Empty instruction (is inserted when an instruction is deleted)
 */
Instruction emptyInstruction = {0,0,0,0};


/**
  * @brief Deletes the instruction at the given position
  * @param pPos Position of the instruction
  */
void InstructionList_RemoveInstruction(uint8_t pPos)
{
    for(int i = pPos; i < 0xFF0; i++)
    {
    	Instruction in = EEPROM_GetInstruction(i+1);
    	EEPROM_PutInstruction(in,i);
    	if(in.functionNumber == FUNCTION_EMPTY) break;
    }
}


/**
  * @brief Inserts an instruction at the given position
  * @param pPos Position at which to insert the instruction
  */
void InstructionList_InsertEmpty(uint8_t pPos)
{
	int firstEmpty = 0;
	while(EEPROM_GetFunctionNumber(firstEmpty) != FUNCTION_EMPTY){firstEmpty++;}
    for(int i = firstEmpty; i > pPos; i--)
    {
    	Instruction in = EEPROM_GetInstruction(i-1);
		EEPROM_PutInstruction(in,i);
    }

    EEPROM_PutInstruction(emptyInstruction, pPos);

}

/**
  * @brief Writes the 3-character identifier of a given instruction into a char-array
  * @param pIn The instruction to be processed
  * @param arr The array in which to write the identifier
  */
void InstructionList_GetFunctionName(Instruction pIn, char arr[3])
{
    for (uint16_t i = 0; i < sizeof(functionNames) / sizeof(FunctionNameEntry); i++) {
        if (pIn.functionNumber == functionNames[i].functionNumber) {
            arr[0] = functionNames[i].name[0];
            arr[1] = functionNames[i].name[1];
            arr[2] = functionNames[i].name[2];
            return;
        }
    }

    arr[0] = 'X'; arr[1] = 'X'; arr[2] = 'X';
}

/**
  * @brief Writes the contents of two instructions onto the display together with line numbers
  * @param i1 The first instruction to be displayed
  * @param i2 The second instruction to be displayed
  * @param pIndex Index of the first instruction (used for line numbers)
  */
void InstructionList_WriteInstructions(Instruction i1, Instruction i2, uint16_t pIndex)
{
	char functionName[3];
	char numChars[3];
	InstructionList_GetFunctionName(i1,functionName);
	STM_Number3ToChar(pIndex, numChars);
	char instChars[] =
	{
			numChars[0],numChars[1],numChars[2],' ',':',' ',
			functionName[0],functionName[1],functionName[2],' ',
			i1.data,i1.data2,i1.data3
	};

	InstructionList_GetFunctionName(i2,functionName);
	STM_Number3ToChar(pIndex+1, numChars);
	char instChars2[] =
	{
			numChars[0],numChars[1],numChars[2],' ',':',' ',
			functionName[0],functionName[1],functionName[2],' ',
			i2.data,i2.data2,i2.data3
	};

	Display_WriteString(instChars, sizeof(instChars), 0, 0);
    Display_WriteString(instChars2, sizeof(instChars2), 0, 2);
}

/**
  * @brief 	Writes the instruction pointed at by the index variable (programIndex) and the instruction next to it onto the display
  together with line numbers and an arrow pointing at the active iinstruction.
  * @details The active instruction will always be the one on the bottom, except for when the index (programIndex) is zero.
  In this case the arrow will point at line number zero at the top, with line 1 being underneith it.
  */
void InstructionList_UpdateInstructions()
{
	InstructionList_WriteInstructions(EEPROM_GetInstruction(programIndex-(programIndex != 0)),EEPROM_GetInstruction(programIndex+1-(programIndex != 0)),programIndex-(programIndex != 0));
	Display_LeftArrow(3*(programIndex != 0));
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
void InstructionList_EvaluateCondition(bool condition) {

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
  * @brief Writes up to 3 characters at the current position of the cursor variable (cursPos)
  * @param str Characters to be written (character that equal 0 will be ignored)
  */
void InstructionList_WriteAtCursor(char str[3])
{
	Display_WriteString(str, 3, cursPos, 0);
	for(uint8_t i = 0; i < 3; i++)
	{
		if(str[i] != 0)cursPos++;
	}
}


/**
  * @brief Return the function number corresponding to the given characters
  * @param chars The characters representing the function
  * @return The function number corresponding to the characters (or 255 if none was found)
  */
uint8_t InstructionList_CharsToFunctionNumber(char chars[3])
{
	for (uint16_t i = 0; i < sizeof(functionNames) / sizeof(FunctionNameEntry); i++)
	{
		if (chars[0] == functionNames[i].name[0]&&chars[1] == functionNames[i].name[1]&&chars[2] == functionNames[i].name[2])
		{
			return(functionNames[i].functionNumber);
		}
	}
	return(255);

}



/**
  * @brief Executes the function of the instruction pointed to by the index (programIndex)
  */
void InstructionList_ExecuteNext() {
	Instruction exe = EEPROM_GetInstruction(programIndex);
	programIndex++;
	if (exe.data == 'R') {
		uint8_t regNumber;
		if (exe.data3 >= 58 || exe.data3 <= 48)
			regNumber = exe.data2 - 48;
		else if (exe.data2 >= 58 || exe.data2 <= 48)
			regNumber = ((exe.data2 - 48) * 10 + exe.data3 - 48);

		switch (exe.functionNumber) {
		case FUNCTION_PIC:
			regPointer = regNumber;
			break;
		case FUNCTION_COP:
			registers[regNumber] = registers[regPointer];
			break;
		case FUNCTION_ADD:
			registers[regPointer] += registers[regNumber];
			break;
		case FUNCTION_SUB:
			registers[regPointer] -= registers[regNumber];
			break;
		case FUNCTION_SMA:
			InstructionList_EvaluateCondition(registers[regPointer] < registers[regNumber]);
			break;
		case FUNCTION_BIG:
			InstructionList_EvaluateCondition(registers[regPointer] > registers[regNumber]);
			break;
		case FUNCTION_REQ:
			InstructionList_EvaluateCondition(registers[regPointer] == registers[regNumber]);
			break;
		case FUNCTION_RNQ:
			InstructionList_EvaluateCondition(registers[regPointer] != registers[regNumber]);
			break;
		case FUNCTION_PTR:
			char str[3];
			STM_Number3ToChar(registers[regNumber], str);
			InstructionList_WriteAtCursor(str);
			break;
		case FUNCTION_SPO:
			registers[regNumber] = programIndex-1;
			break;
		case FUNCTION_JPO:
			programIndex = registers[regNumber];
			break;
		default:
			Display_ShowErrorMessage(programIndex-1);
			break;
		}
	}
	else
	{
		uint8_t datNumber = 0;
		if (exe.data >= 48 && exe.data <= 58) {
			if (exe.data3 >= 48 && exe.data3 <= 58) {
				datNumber = ((exe.data - 48) * 100 + (exe.data2 - 48) * 10
						+ exe.data3 - 48);
			} else if (exe.data2 >= 48 && exe.data2 <= 58) {
				datNumber = ((exe.data - 48) * 10 + exe.data2 - 48);
			} else if (exe.data >= 48 && exe.data <= 58) {
				datNumber = (exe.data - 48);
			}
		}

		switch (exe.functionNumber) {
		case FUNCTION_SET:
			registers[regPointer] = datNumber;
			break;
		case FUNCTION_WAI:
			STM_Wait(datNumber);
			break;
		case FUNCTION_INC:
			registers[regPointer] = registers[regPointer] + datNumber;
			break;
		case FUNCTION_DEC:
			registers[regPointer] = registers[regPointer] - datNumber;
			break;
		case FUNCTION_VEQ:
			InstructionList_EvaluateCondition(datNumber == registers[regPointer]);
			break;
		case FUNCTION_VNQ:
			InstructionList_EvaluateCondition(!(datNumber == registers[regPointer]));
			break;
		case FUNCTION_TON:
			STM_ActivateBuzzer(exe);
			break;
		case FUNCTION_ANH:
			InstructionList_EvaluateCondition(registers[regPointer] < STM_ReadADC(datNumber));
			break;
		case FUNCTION_ANL:
			InstructionList_EvaluateCondition(!(registers[regPointer] < STM_ReadADC(datNumber)));
			break;
		case FUNCTION_SVA:
			registers[regPointer] = STM_ReadADC(datNumber);
			break;
		case FUNCTION_INH:
			InstructionList_EvaluateCondition(STM_IsInputHigh(datNumber));
			break;
		case FUNCTION_INL:
			InstructionList_EvaluateCondition(!(STM_IsInputHigh(datNumber)));
			break;
		case FUNCTION_JUM:
			programIndex = datNumber;
			break;
		case FUNCTION_LD1:
		case FUNCTION_LD2:
			STM_SetLED(exe.functionNumber, exe.data);
			break;
		case FUNCTION_PCH:
			char str[] = {exe.data, exe.data2, exe.data3};
			InstructionList_WriteAtCursor(str);
			break;
		case FUNCTION_CLR:
			cursPos = 0;
			Display_FillBlack();
			break;
		case FUNCTION_EMPTY:
			break;
		default:
			Display_ShowErrorMessage(programIndex-1);
			break;
		}
	}
}


//Documented in .h
void InstructionList_Init()
{
    for(uint16_t i = 0; i < 100; i++)
    {
    	registers[i] = 0;
    }
}


//Documented in .h
void InstructionList_ProgrammingMode(void)
{

    if(!(isProgrammingMode()))
        return;

    STM_ActivateBuzzer(emptyInstruction);
    STM_SetLED(FUNCTION_LD1, 'A');
    STM_SetLED(FUNCTION_LD2, 'A');
    Display_ShowProgrammingMessage();

    programIndex = 0;
    while((programIndex < 0x0FF0) && !(EEPROM_GetFunctionNumber(programIndex) == FUNCTION_EMPTY))
        programIndex++;

    Display_FillBlack();
    InstructionList_UpdateInstructions();

    char ch = 0;
    char linePos = 6;
    char instructionKeys[] = {0,0,0,0,0,0,0};

    while(isProgrammingMode())
    {
		while(ch == 0)
		{
			ch = PS2_GetKey();
			if(!isProgrammingMode())
				return;
		}
		Instruction in;
		if(ch == ']')
		{
			if(instructionKeys[0] != 0)
			{
				char fName[] = {instructionKeys[0],instructionKeys[1],instructionKeys[2]};
				in.functionNumber = InstructionList_CharsToFunctionNumber(fName);

				in.data = instructionKeys[4];
				in.data2 = instructionKeys[5];
				in.data3 = instructionKeys[6];
				EEPROM_PutInstruction(in,programIndex);
			}
			programIndex++;
		}
		else if(ch == '^' && programIndex > 0)
		{
			programIndex--;
		}
		else if(ch == '.')
		{
			InstructionList_InsertEmpty(programIndex);
		}
		else if(ch == ',')
		{
			InstructionList_RemoveInstruction(programIndex);
		}
		else if(ch == '<')
		{
			if(linePos > 6)
			{
				instructionKeys[linePos-7] = 0;
				Display_WriteCharacter(' ', --linePos,(programIndex != 0)*2);
			}
		}
		else
		{
			if(linePos == 6)
				for(int i = 0; i < 9; i++)
				{
					Display_WriteCharacter(' ',6+i,(programIndex!=0)*2);
				}

			if(linePos < 13)
			{
				instructionKeys[linePos-6] = ch;
				Display_WriteCharacter(ch, linePos++,(programIndex!=0)*2);
			}
		}

		if(ch == ']' || ch == '^' || ch == '.' || ch == ',')
		{
			InstructionList_UpdateInstructions();

			for(int i = 0; i < 7; i++)
			{
				instructionKeys[i] = 0;
			}

			linePos = 6;
		}

		keyBuffer = 0;
		framePos = 0;
		ch = 0;

    }

}

//Documented in .h
void InstructionList_ExecutingMode(void)
{
    if(isProgrammingMode())
        return;

    Display_ShowExecutingMessage();

    for(uint8_t i = 0; i < 100; i++)
    {
    	registers[i] = 0;
    }
    programIndex = 0;
    regPointer = 0;
    cursPos = 0;

    lastWaitTick = HAL_GetTick();
    while(!(isProgrammingMode()))
    {
        if(EEPROM_GetFunctionNumber(programIndex) == FUNCTION_EMPTY || programIndex > 0xFF0)
            Display_ShowTerminatedMessage();
        else
            InstructionList_ExecuteNext();
    }
}

