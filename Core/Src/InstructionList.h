/**
 * @file InstructionList.h
 * @brief Provides functions for the programming and execution mode to other files as well as the function identifiers.
 * @details To add your own functions to the system, first insert FUNCTION_XXX into the typedef enum, then add the function to
 * "definedFunctions[]" using the FUNCTION_XXX you just put in above as well as a function Identifier (normally this is the XXX).
 * Then go on and add the function handler to definedFunctions[]. The function handler must then be defined in InstructionHandlers.h
 * and implemented in InstructionHandlers.c.
 */

#ifndef INSTRUCTIONLIST_H
#define	INSTRUCTIONLIST_H
#include "Instruction.h"
#include "InstructionHandlers.h"

/**
  * @brief 	Allows for programming the PCD if set into programming mode. Writes instructions into EEPROM
  */
void InstructionList_ProgrammingMode(void);


/**
  * @brief Executes commands from the eeprom begining at position 0.
  Stops only if an error occurs, an empty function is encountered or the divice is switched back into programming mode.
  */
void InstructionList_ExecutingMode(void);







#endif
