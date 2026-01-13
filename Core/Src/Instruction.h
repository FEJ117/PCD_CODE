/**
 * @file Instruction.h
 * @brief Definiert Struct Instruction
 */

#ifndef INSTRUCTION_H
#define INSTRUCTION_H
/**
 * @brief Struct zum Speichern einer Instruction mit Funktionsnummer und 3 Bytes Daten
 */
typedef struct {
	/**
	 * @brief Erstes Datenfeld (1 Byte)
	 */
    uint8_t data;

    /**
	 * @brief Zweites Datenfeld (1 Byte)
	 */
    uint8_t data2;

    /**
	 * @brief Drittes Datenfeld (1 Byte)
	 */
    uint8_t data3;

    /**
	 * @brief Funktionsnummer der Instruction
	 */
    uint8_t functionNumber;
} Instruction;
#endif
