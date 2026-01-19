/**
 * @file InstructionList.c
 * @brief Implementation of the main program logic for writing and executing commands
 * @details To add your own functions to the system, first insert FUNCTION_XXX into the typedef enum at the end of the file,
 * then add the function to "definedFunctions[]" using the FUNCTION_XXX you just put in above as well as a function Identifier (normally this is the XXX).
 * Then go on and add the function handler to definedFunctions[]. The function handler must then be defined in InstructionHandlers.h
 * and implemented in InstructionHandlers.c.
 */

#include "EEPROM.h"
#include "Instruction.h"
#include "PS2Driver.h"
#include "InstructionList.h"
#include "STM_FUNCTIONS.h"

/**
 * @brief Current position in the programm (both used in programming and executing)
 */
uint16_t programIndex = 0;

/**
 * @brief Empty instruction (is inserted when an instruction is deleted)
 */
Instruction emptyInstruction = {0,0,0,0};


/**
  * @brief Deletes the instruction at the given position
  * @param pPos Position of the instruction
  */
static void InstructionList_RemoveInstruction(uint8_t pPos)
{
    for(int i = pPos; i < 0xFF0; i++)
    {
    	Instruction in = EEPROM_GetInstruction(i+1);
    	EEPROM_PutInstruction(in,i);
    	if(in.functionNumber == FUNCTION_EMP) break;
    }
}


/**
  * @brief Inserts an instruction at the given position
  * @param pPos Position at which to insert the instruction
  */
static void InstructionList_InsertEmpty(uint8_t pPos)
{
	int firstEmpty = 0;
	while(EEPROM_GetFunctionNumber(firstEmpty) != FUNCTION_EMP){firstEmpty++;}
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
static void InstructionList_GetFunctionName(Instruction pIn, char arr[3])
{
	if(pIn.functionNumber < Function_t_MAX)
	{
		arr[0] = definedFunctions[pIn.functionNumber].name[0];
		arr[1] = definedFunctions[pIn.functionNumber].name[1];
		arr[2] = definedFunctions[pIn.functionNumber].name[2];
		return;
	}
}

/**
  * @brief Writes the contents of two instructions onto the display together with line numbers
  * @param i1 The first instruction to be displayed
  * @param i2 The second instruction to be displayed
  * @param pIndex Index of the first instruction (used for line numbers)
  */
static void InstructionList_WriteInstructions(Instruction i1, Instruction i2, uint16_t pIndex)
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
static void InstructionList_UpdateInstructions()
{
	InstructionList_WriteInstructions(EEPROM_GetInstruction(programIndex-(programIndex != 0)),EEPROM_GetInstruction(programIndex+1-(programIndex != 0)),programIndex-(programIndex != 0));
	Display_LeftArrow(3*(programIndex != 0));
}


/**
  * @brief Return the function number corresponding to the given characters
  * @param chars The characters representing the function
  * @return The function number corresponding to the characters (or 255 if none was found)
  */
static uint8_t InstructionList_CharsToFunctionNumber(char chars[3])
{
	for (uint16_t i = 0; i < sizeof(definedFunctions) / sizeof(FunctionDefinition); i++)
	{
		if (chars[0] == definedFunctions[i].name[0]&&chars[1] == definedFunctions[i].name[1]&&chars[2] == definedFunctions[i].name[2])
		{
			return(i);
		}
	}
	return(255);

}



/**
  * @brief Executes the function of the instruction pointed to by the index (programIndex)
  */
static void InstructionList_ExecuteNext() {
	Instruction exe = EEPROM_GetInstruction(programIndex);
	programIndex++;
	InstructionHandlers_ProcessData(&exe);
	definedFunctions[exe.functionNumber].handler(&exe);
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
    while((programIndex < 0x0FF0) && !(EEPROM_GetFunctionNumber(programIndex) == FUNCTION_EMP))
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

    InstructionHandlers_INIT();
    while(!(isProgrammingMode()))
    {
        if(EEPROM_GetFunctionNumber(programIndex) == FUNCTION_EMP || programIndex > 0xFF0)
            Display_ShowTerminatedMessage();
        else
            InstructionList_ExecuteNext();
    }
}

