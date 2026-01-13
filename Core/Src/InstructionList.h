/**
 * @file InstructionList.h
 * @brief Enthält Funktionen zum Programmieren des PCD und Ausführen des Programms
 */

#ifndef INSTRUCTIONLIST_H
#define	INSTRUCTIONLIST_H
#include "Instruction.h"
/**
 * @brief Definiert alle Funktionsbezeichner und weist ihnen eine Nummer zu
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
 * @brief Struct, die einer Funktionsnummer einen Funktionsbezeichner aus 3 Zeichen zuordnet
 */
typedef struct {
	/**
	 * @brief Funktionsnummer
	 */
    uint8_t functionNumber;

    /**
     * @brief Funktionsbezeichner bestehend aus 3 Zeichen
     */
    const char name[3];
} FunctionNameEntry;

/**
 * @brief Alle Funktionsnummern und zugehörige Funktionsbezeichner
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
 * @brief Aktuelle Programmposition (sowohl bei der Programmierung, als auch bei Ausführen des Programms)
 */
extern uint16_t ind;

/**
  * @brief Setzt alle Register auf 0
  */
void init();

/**
  * @brief 	Erlaubt das Programmieren über die Tastatur wenn der Modus auf "Programmieren" gestellt ist.
  * 		Wertet die Tastatureingaben aus und schreibt, beziehungsweise löscht entsprechend Daten des EEPROM
  */
void programmingMode(void);


/**
  * @brief 	Wird aufgerufen, wenn vom Programmiermodus in den Ausführen-Modus gewechselt wird.
  * 		Setzt alle Register, sowie den Index zurück und führt solange Instructions aus,
  * 		bis das Programmende erreicht ist oder der Modus gewechselt wird.
  */
void executionMode(void);







#endif
