/**
 * @file InstructionList.h
 * @brief Provides functions for the programming and execution mode to other files as well as the function identifiers
 */

#ifndef INSTRUCTIONLIST_H
#define	INSTRUCTIONLIST_H
#include "Instruction.h"
/**
 * @brief Defines all function numbers
 */
typedef enum {
    FUNCTION_EMPTY = 0,
    FUNCTION_PIC,
    FUNCTION_SET,
    FUNCTION_INC,
    FUNCTION_DEC,
    FUNCTION_COP,
    FUNCTION_ADD,
    FUNCTION_SUB,
    FUNCTION_SMA,
    FUNCTION_BIG,
    FUNCTION_REQ,
    FUNCTION_RNQ,
    FUNCTION_VEQ,
    FUNCTION_VNQ,
    FUNCTION_AOU,
    FUNCTION_DOU,
    FUNCTION_ANH,
    FUNCTION_ANL,
    FUNCTION_SVA,
    FUNCTION_INH,
    FUNCTION_INL,
    FUNCTION_TON,
    FUNCTION_PTR,
    FUNCTION_PCH,
    FUNCTION_CLR,
    FUNCTION_BEG,
    FUNCTION_END,
    FUNCTION_WAI,
    FUNCTION_SPO,
    FUNCTION_JPO,
    FUNCTION_JUM,
    FUNCTION_LD1,
    FUNCTION_LD2
} Function_t;

/**
 * @brief Struct which allows for allocating a function number to a function identifier
 */
typedef struct {
	/**
	 * @brief Function number
	 */
    uint8_t functionNumber;

    /**
     * @brief Function identifier
     */
    const char name[3];
} FunctionNameEntry;

/**
 * @brief All function numbers and allocated function identifiers
 */
static const FunctionNameEntry functionNames[] = {
	{ FUNCTION_EMPTY, { ' ', ' ', ' ' } },
	{ FUNCTION_PIC,   { 'P', 'I', 'C' } },
	{ FUNCTION_SET,   { 'S', 'E', 'T' } },
	{ FUNCTION_INC,   { 'I', 'N', 'C' } },
	{ FUNCTION_DEC,   { 'D', 'E', 'C' } },
	{ FUNCTION_COP,   { 'C', 'O', 'P' } },
	{ FUNCTION_ADD,   { 'A', 'D', 'D' } },
	{ FUNCTION_SUB,   { 'S', 'U', 'B' } },
	{ FUNCTION_SMA,   { 'S', 'M', 'A' } },
	{ FUNCTION_BIG,   { 'B', 'I', 'G' } },
	{ FUNCTION_REQ,   { 'R', 'E', 'Q' } },
	{ FUNCTION_RNQ,   { 'R', 'N', 'Q' } },
	{ FUNCTION_VEQ,   { 'V', 'E', 'Q' } },
	{ FUNCTION_VNQ,   { 'V', 'N', 'Q' } },
	{ FUNCTION_AOU,   { 'A', 'O', 'U' } },
	{ FUNCTION_DOU,   { 'D', 'O', 'U' } },
	{ FUNCTION_ANH,   { 'A', 'N', 'H' } },
	{ FUNCTION_ANL,   { 'A', 'N', 'L' } },
	{ FUNCTION_INH,   { 'I', 'N', 'H' } },
	{ FUNCTION_INL,   { 'I', 'N', 'L' } },
	{ FUNCTION_TON,   { 'T', 'O', 'N' } },
	{ FUNCTION_WAI,   { 'W', 'A', 'I' } },
	{ FUNCTION_JUM,   { 'J', 'U', 'M' } },
	{ FUNCTION_PTR,   { 'P', 'T', 'R' } },
	{ FUNCTION_PCH,   { 'P', 'C', 'H' } },
	{ FUNCTION_CLR,   { 'C', 'L', 'R' } },
	{ FUNCTION_BEG,   { 'B', 'E', 'G' } },
	{ FUNCTION_END,   { 'E', 'N', 'D' } },
	{ FUNCTION_SPO,   { 'S', 'P', 'O' } },
	{ FUNCTION_JPO,   { 'J', 'P', 'O' } },
	{ FUNCTION_SVA,   { 'S', 'V', 'A' } },
	{ FUNCTION_LD1,   { 'L', 'D', '1' } },
	{ FUNCTION_LD2,   { 'L', 'D', '2' } }
};

/**
 * @brief Current position in the programm (both used in programming and executing)
 */
extern uint16_t programIndex;

/**
  * @brief Sets all registers to 0
  */
void InstructionList_Init();

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
