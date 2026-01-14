/**
 * @file Instruction.h
 * @brief Defines the struct "Instruction"
 */

#ifndef INSTRUCTION_H
#define INSTRUCTION_H
/**
 * @brief Struct to store an instruction containing a function number and 3 bytes of data
 */
typedef struct {
	/**
	 * @brief First byte of data
	 */
    uint8_t data;

    /**
	 * @brief Second byte of data
	 */
    uint8_t data2;

    /**
	 * @brief Third byte of data
	 */
    uint8_t data3;

    /**
	 * @brief Function number
	 */
    uint8_t functionNumber;
} Instruction;
#endif
