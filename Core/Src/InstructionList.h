/**
 * @file InstructionList.h
 * @brief Provides functions for the programming and execution mode to other files as well as the function identifiers
 */

#ifndef INSTRUCTIONLIST_H
#define	INSTRUCTIONLIST_H
#include "Instruction.h"
#include "InstructionHandlers.h"
/**
 * @brief Defines all function numbers
 */
typedef enum {
    FUNCTION_EMP = 0,
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
    FUNCTION_LD2,
    //ADD your own here
    Function_t_MAX
} Function_t;

typedef void (*InstructionHandler)(Instruction *exe);

/**
 * @brief Struct which allows for allocating a function number to a function identifier
 */
typedef struct {
  /**
    * @brief Function identifier
    */
  const char name[3];

  /**
  * @brief The function that is called when the function is being executed
  */
  InstructionHandler handler;

    
} FunctionDefinition;

/**
 * @brief All function numbers and allocated function identifiers
 */
static const FunctionDefinition definedFunctions[] = {
  [FUNCTION_EMP] = {{ ' ', ' ', ' ' }, op_EMP_BEG_END},
  [FUNCTION_PIC] = {{ 'P', 'I', 'C' }, op_PIC},
  [FUNCTION_SET] = {{ 'S', 'E', 'T' }, op_SET},
  [FUNCTION_INC] = {{ 'I', 'N', 'C' }, op_INC_DEC},
  [FUNCTION_DEC] = {{ 'D', 'E', 'C' }, op_INC_DEC},
  [FUNCTION_COP] = {{ 'C', 'O', 'P' }, op_COP},
  [FUNCTION_ADD] = {{ 'A', 'D', 'D' }, op_ADD_SUB},
  [FUNCTION_SUB] = {{ 'S', 'U', 'B' }, op_ADD_SUB},
  [FUNCTION_SMA] = {{ 'S', 'M', 'A' }, op_SMA_BIG},
  [FUNCTION_BIG] = {{ 'B', 'I', 'G' }, op_SMA_BIG},
  [FUNCTION_REQ] = {{ 'R', 'E', 'Q' }, op_REQ_RNQ},
  [FUNCTION_RNQ] = {{ 'R', 'N', 'Q' }, op_REQ_RNQ},
  [FUNCTION_VEQ] = {{ 'V', 'E', 'Q' }, op_VEQ_VNQ},
  [FUNCTION_VNQ] = {{ 'V', 'N', 'Q' }, op_VEQ_VNQ},
  [FUNCTION_ANH] = {{ 'A', 'N', 'H' }, op_ANH_ANL},
  [FUNCTION_ANL] = {{ 'A', 'N', 'L' }, op_ANH_ANL},
  [FUNCTION_SVA] = {{ 'S', 'V', 'A' }, op_SVA},
  [FUNCTION_INH] = {{ 'I', 'N', 'H' }, op_INH_INL},
  [FUNCTION_INL] = {{ 'I', 'N', 'L' }, op_INH_INL},
  [FUNCTION_TON] = {{ 'T', 'O', 'N' }, op_TON},
  [FUNCTION_PTR] = {{ 'P', 'T', 'R' }, op_PTR},
  [FUNCTION_PCH] = {{ 'P', 'C', 'H' }, op_PCH},
  [FUNCTION_CLR] = {{ 'C', 'L', 'R' }, op_CLR},
  [FUNCTION_BEG] = {{ 'B', 'E', 'G' }, op_EMP_BEG_END},
  [FUNCTION_END] = {{ 'E', 'N', 'D' }, op_EMP_BEG_END},
  [FUNCTION_WAI] = {{ 'W', 'A', 'I' }, op_WAI},
  [FUNCTION_SPO] = {{ 'S', 'P', 'O' }, op_SPO},
  [FUNCTION_JPO] = {{ 'J', 'P', 'O' }, op_JPO},
  [FUNCTION_JUM] = {{ 'J', 'U', 'M' }, op_JUM},
  [FUNCTION_LD1] = {{ 'L', 'D', '1' }, op_LD1_LD2},
  [FUNCTION_LD2] = {{ 'L', 'D', '2' }, op_LD1_LD2}
  //Add your own here
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
