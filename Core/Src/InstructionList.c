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
uint16_t ind = 0;

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
void removeInstruction(uint8_t pPos)
{
    for(int i = pPos; i < 0xFF0; i++)
    {
    	Instruction in = getInstruction(i+1);
    	putInstruction(in,i);
    	if(in.functionNumber == FUNCTION_EMPTY) break;
    }
}


/**
  * @brief Inserts an instruction at the given position
  * @param pPos Position at which to insert the instruction
  */
void insertInstruction(uint8_t pPos)
{
	int firstEmpty = 0;
	while(getFunctionNumberByPosition(firstEmpty) != FUNCTION_EMPTY){firstEmpty++;}
    for(int i = firstEmpty; i > pPos; i--)
    {
    	Instruction in = getInstruction(i-1);
		putInstruction(in,i);
    }

    putInstruction(emptyInstruction, pPos);

}

/**
  * @brief Writes the 3-character identifier of a given instruction into a char-array
  * @param pIn The instruction to be processed
  * @param arr The array in which to write the identifier
  */
void GetFunctionName(Instruction pIn, char arr[3])
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
void writeInstructions(Instruction i1, Instruction i2, uint16_t pIndex)
{
	char functionName[3];
	char numChars[3];
	GetFunctionName(i1,functionName);
	number3ToChar(pIndex, numChars);
	char instChars[] =
	{
			numChars[0],numChars[1],numChars[2],' ',':',' ',
			functionName[0],functionName[1],functionName[2],' ',
			i1.data,i1.data2,i1.data3
	};

	GetFunctionName(i2,functionName);
	number3ToChar(pIndex+1, numChars);
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
  * @brief 	Writes the instruction pointed at by the index variable (ind) and the instruction next to it onto the display
  together with line numbers and an arrow pointing at the active iinstruction.
  * @details The active instruction will always be the one on the bottom, except for when the index (ind) is zero.
  In this case the arrow will point at line number zero at the top, with line 1 being underneith it.
  */
void updateInstructions()
{
	writeInstructions(getInstruction(ind-(ind != 0)),getInstruction(ind+1-(ind != 0)),ind-(ind != 0));
	writeLeftArrow(3*(ind != 0));
}

/**
  * @brief 	Evaluates, if a condition is true
  * 		- If BEG and END are being used:
  * 			# If the condition is true: Jump into the block (one position after BEG)
  * 			# If the condition is false: Jump one position after END
* 			- If BEG and END are not being used:
* 				# If the condition is true: Jumps to the next instruction
* 				# If the contition is false: Jumps to the instruction after the next
  * @param condition The condition which is being processed (This is technically a bool, but this datatype is not being used)
  */
void evaluate(uint8_t condition) {

	if(!condition)
	{
		uint8_t openedBrackets = 1;
		if (getFunctionNumberByPosition(ind) == FUNCTION_BEG) {
			while (getFunctionNumberByPosition(ind) != FUNCTION_END
					|| openedBrackets > 0) {
				ind++;
				if (getFunctionNumberByPosition(ind) == FUNCTION_BEG)
					openedBrackets++;
				if (getFunctionNumberByPosition(ind) == FUNCTION_END)
					openedBrackets--;
			}
		}
		ind++;
	}
}


/**
  * @brief Writes up to 3 characters at the current position of the cursor variable (cursPos)
  * @param str Characters to be written (character that equal 0 will be ignored)
  */
void writeCharsAtCursorPosition(char str[3])
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
uint8_t charsToFunctionNumber(char chars[3])
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
  * @brief Executes the function of the instruction pointed to by the index (ind)
  */
void executeInstruction() {
	Instruction exe = getInstruction(ind);
	ind++;
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
			evaluate(registers[regPointer] < registers[regNumber]);
			break;
		case FUNCTION_BIG:
			evaluate(registers[regPointer] > registers[regNumber]);
			break;
		case FUNCTION_REQ:
			evaluate(registers[regPointer] == registers[regNumber]);
			break;
		case FUNCTION_RNQ:
			evaluate(registers[regPointer] != registers[regNumber]);
			break;
		case FUNCTION_PTR:
			char str[3];
			number3ToChar(registers[regNumber], str);
			writeCharsAtCursorPosition(str);
			break;
		case FUNCTION_SPO:
			registers[regNumber] = ind-1;
			break;
		case FUNCTION_JPO:
			ind = registers[regNumber];
			break;
		default:
			fehlerMeldung(ind-1);
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
			wait(datNumber);
			break;
		case FUNCTION_INC:
			registers[regPointer] = registers[regPointer] + datNumber;
			break;
		case FUNCTION_DEC:
			registers[regPointer] = registers[regPointer] - datNumber;
			break;
		case FUNCTION_VEQ:
			evaluate(datNumber == registers[regPointer]);
			break;
		case FUNCTION_VNQ:
			evaluate(!(datNumber == registers[regPointer]));
			break;
		case FUNCTION_TON:
			activateBuzzer(exe);
			break;
		case FUNCTION_ANH:
			evaluate(registers[regPointer] < STM_readADC(datNumber));
			break;
		case FUNCTION_ANL:
			evaluate(!(registers[regPointer] < STM_readADC(datNumber)));
			break;
		case FUNCTION_SVA:
			registers[regPointer] = STM_readADC(datNumber);
			break;
		case FUNCTION_INH:
			evaluate(isInputHigh(datNumber));
			break;
		case FUNCTION_INL:
			evaluate(!(isInputHigh(datNumber)));
			break;
		case FUNCTION_JUM:
			ind = datNumber;
			break;
		case FUNCTION_LD1:
		case FUNCTION_LD2:
			setLED(exe.functionNumber, exe.data);
			break;
		case FUNCTION_PCH:
			char str[] = {exe.data, exe.data2, exe.data3};
			writeCharsAtCursorPosition(str);
			break;
		case FUNCTION_CLR:
			cursPos = 0;
			FillBlack();
			break;
		case FUNCTION_EMPTY:
			break;
		default:
			fehlerMeldung(ind-1);
			break;
		}
	}
}


//Documented in .h
void init()
{
    for(uint16_t i = 0; i < 100; i++)
    {
    	registers[i] = 0;
    }
}


//Documented in .h
void programmingMode(void)
{

    if(!(isProgrammingMode()))
        return;

    activateBuzzer(emptyInstruction);
    setLED(FUNCTION_LD1, 'A');
    setLED(FUNCTION_LD2, 'A');
    showProgrammingModeMessage();

    ind = 0;
    while((ind < 0x0FF0) && !(getFunctionNumberByPosition(ind) == FUNCTION_EMPTY))
        ind++;

    FillBlack();
    updateInstructions();

    char ch = 0;
    char linePos = 6;
    char instructionKeys[] = {0,0,0,0,0,0,0};

    while(isProgrammingMode())
    {
		while(ch == 0)
		{
			ch = getKey();
			if(!isProgrammingMode())
				return;
		}
		Instruction in;
		if(ch == ']')
		{
			if(instructionKeys[0] != 0)
			{
				char fName[] = {instructionKeys[0],instructionKeys[1],instructionKeys[2]};
				in.functionNumber = charsToFunctionNumber(fName);

				in.data = instructionKeys[4];
				in.data2 = instructionKeys[5];
				in.data3 = instructionKeys[6];
				putInstruction(in,ind);
			}
			ind++;
		}
		else if(ch == '^' && ind > 0)
		{
			ind--;
		}
		else if(ch == '.')
		{
			insertInstruction(ind);
		}
		else if(ch == ',')
		{
			removeInstruction(ind);
		}
		else if(ch == '<')
		{
			if(linePos > 6)
			{
				instructionKeys[linePos-7] = 0;
				Display_WriteCharacter(' ', --linePos,(ind != 0)*2);
			}
		}
		else
		{
			if(linePos == 6)
				for(int i = 0; i < 9; i++)
				{
					Display_WriteCharacter(' ',6+i,(ind!=0)*2);
				}

			if(linePos < 13)
			{
				instructionKeys[linePos-6] = ch;
				Display_WriteCharacter(ch, linePos++,(ind!=0)*2);
			}
		}

		if(ch == ']' || ch == '^' || ch == '.' || ch == ',')
		{
			updateInstructions();

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
void executionMode(void)
{
    if(isProgrammingMode())
        return;

    showExecutionModeMessage();

    for(uint8_t i = 0; i < 100; i++)
    {
    	registers[i] = 0;
    }
    ind = 0;
    regPointer = 0;
    cursPos = 0;

    lastTick = HAL_GetTick();
    while(!(isProgrammingMode()))
    {
        if(getFunctionNumberByPosition(ind) == FUNCTION_EMPTY || ind > 0xFF0)
            showProgramTerminatedMessage();
        else
            executeInstruction();
    }
}

