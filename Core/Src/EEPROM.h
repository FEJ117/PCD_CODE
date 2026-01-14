/**
 * @file BA_EEPROM.h
 * @brief Provides functions for the use of the EEPROM to other files
 */


#ifndef SRC_BA_EEPROM_H_
#define SRC_BA_EEPROM_H_
#include<stdint.h>
#include"main.h"
#include"Instruction.h"


/**
  * @brief Reads an instruction from the EEPROM
  * @details 0 equals the first Instruction 1 the seconds and so on.
  Even though the instructions take up 4 bytes of space this is accounted for inside the function
  * @param position Position of the instruction
  * @return Read instruction
  */
Instruction EEPROM_GetInstruction(int position);


/**
  * @brief Reads the function number of an instruction in the EEPROM
  * @details Position 0 equals the first instruction, 1 the seconds and so on.
  Even though the instructions take up 4 bytes of space this is accounted for inside the function
  * @param position Position of the instruction
  * @return Function number of the instruction
  */
uint8_t EEPROM_GetFunctionNumber(int position);


/**
  * @brief Writes an instruction into EEPROM at the given position
  * @details Position 0 equals the first instruction, 1 the seconds and so on.
  Even though the instructions take up 4 bytes of space this is accounted for inside the function
  * @param in The instruction to be written
  * @param position The position at which to write the Instruction.
  */
void EEPROM_PutInstruction(Instruction in, int position);


/**
  * @brief Sets all bits of the EEPROM to zero
  */
void EEPROM_EraseAll();


#endif
