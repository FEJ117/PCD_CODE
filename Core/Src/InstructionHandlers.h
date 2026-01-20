/**
 * @file InstructionHandlers.h
 * @brief Provides all instruction handlers as well as helper functions to outside files.
 */

#ifndef INSTRUCTIONHANDLERS_H
#define	INSTRUCTIONHANDLERS_H

#include "Instruction.h"


/**
* @brief Initializes the registers as well as the program index, cursor and register pointer to be 0.
* @warning Should be called everytime the program switches into executing mode
*/
void InstructionHandlers_INIT();

/**
* @brief Processes the data stored in the current instruction and sets the data type processed.
* @details The datatype can be used by the functions to determine if the data provided has the right format.
*/
void InstructionHandlers_ProcessData(Instruction *exe);



//Add your own here



/** @brief Handler for the instructions EMP, BEG and END. Does nothing. 
  * @example The code:\INH 0\BEG\LD1 V\WAI 5\LD1 A\WAI 5\END\JUM 0\
  * will let the left LED blink when the first Button is pressed. Everything between BEG and END
  * is only being executed, if the condition (INH 0) is true.
*/
void op_EMP_BEG_END(Instruction *exe);
    


/** @brief Handler for the instruction PIC. 
  * @details Sets the register pointer to the desired value specified in the data of the instruction.
 */
void op_PIC(Instruction *exe);
    


/** @brief Handler for the instruction SET. 
  * @details Sets the value of the register pointed to by the register pointer
  * to the value specified in the data of the instruction.
 */
void op_SET(Instruction *exe);
    


/** @brief Handler for the instructions INC and DEC. 
  * @details Increments or decrements the register pointed to by the register pointer
  * by the value specified in the data of the instruction.
 */
void op_INC_DEC(Instruction *exe);
    


/** @brief Handler for the instruction COP. 
  * @details Copies the register pointed to by the register pointer into
  * the register specified by the data of the instruction.
 */
void op_COP(Instruction *exe);
    


/** @brief Handler for the instructions ADD and SUB. 
  * @details Adds or subtracts the value of the register specified by the data of the instruction from/to
  * the register pointed to by the register pointer.
 */
void op_ADD_SUB(Instruction *exe);
    

/** @brief Handler for the instructions SMA and BIG. 
  * @details Determines, if the value of the register pointed to by the register pointer is
  * smaller or bigger then the value of the register specified by the data of the instruction.
 */
 void op_SMA_BIG(Instruction *exe);
    


/** @brief Handler for the instructions REQ and RNQ. 
  * @details Determines, if the values of the register pointed to by the register pointer and
  * the value of the register specified by the data of the instruction are equal or not equal.
 */
void op_REQ_RNQ(Instruction *exe);



/** @brief Handler for the instructions VEQ and VNQ. 
  * @details Determines, if the values of the register pointed to by the register pointer and
  * the value of the data of the instruction are equal or not equal.
 */
void op_VEQ_VNQ(Instruction *exe);



/** @brief Handler for the instructions ANH and ANL. 
  * @details Determines, if the values of the register pointed to by the register pointer and
  * the value of the analog input provided by the data of the instuction are smaller or bigger.
  * @warning The value passed in the instruction is the ADC number of the analog input.
 */
void op_ANH_ANL(Instruction *exe);
    


/** @brief Handler for the instructions REQ and RNQ. 
  * @details Determines, if the values of the register pointed to by the register pointer and
  * the value of the register specified by the data of the instruction are equal or not equal.
 */
void op_SVA(Instruction *exe);
    


/** @brief Handler for the instructions INH and INL. 
  * @details Determines, if the input specified by the data of the instruction is high or low.
  * @warning Input0 = Button1, Input1 = Button2, Input2 = Button3, Input3 = Button4
 */
void op_INH_INL(Instruction *exe);
    


/** @brief Handler for the instruction TON. 
  * @details Produces a speaker output representing the tone specified in the instruction's data.
  * @warning The tone must be provided in the following format: C#7, where C is any tone (C, D, E, F, G, A, H),
  * 7 is the pitch (choose from 1-7) and # indicates the semitone higher then C. # is optional.
 */
void op_TON(Instruction *exe);
    


/** @brief Handler for the instruction PTR. 
  * @details Prints the value of the register provided by the data of the instruction to the cursor position
  * and increments the cursor position.
 */
void op_PTR(Instruction *exe);
    


/** @brief Handler for the instruction PCH. 
  * @details Prints any three chars provided by the data of the instruction to the cursor position
  * and increments the cursor position. Chars which are not provided will be left out.
 */
void op_PCH(Instruction *exe);
    


/** @brief Handler for the instruction CLR. 
  * @details Clears the screen.
 */
void op_CLR(Instruction *exe);
    


/** @brief Handler for the instruction WAI. 
  * @details Pauses the program for the amount of time given in the data of the instruction in 1/10 seconds.
 */
void op_WAI(Instruction *exe);
    


/**
  * @brief Handler for the instruction SPO.
  * @details Saves the current position in the program to the register specified by the instruction's data.
*/
void op_SPO(Instruction *exe);
    


/**
  * @brief Handler for the instruction JPO.
  * @details Jumps to the position pointed to by the register specified by the instruction's data.
*/
void op_JPO(Instruction *exe);
    


/**
  * @brief Handler for the instruction JUM.
  * @details Jumps to the position specified in the instruction's data.
*/
void op_JUM(Instruction *exe);
    


/**
  * @brief Handler for the instruction LD1 and LD2.
  * @details Sets one of the LEDs (depending on LD1/LD2) to the colour specified by the instruction's data.
  * (R = Red, G = Green, B = Blue, O = Orange, T = Turquoise, V = Violett, W = White, Any other = OFF)
*/
void op_LD1_LD2(Instruction *exe);



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

/**
* Defines a type for the instruction handlers.
*/
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
  * @brief The handler that is called when the function is being executed
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
    



#endif