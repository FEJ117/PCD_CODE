/**
 * @file DisplayFonts.h
 * @brief Enthält Schriftart zur Textausgabe auf dem Display
 */

/**
 * @file BA_EEPROM.h
 * @brief Stellt Funktionen zur Speicherung und Abfrage von Daten aus dem EEPROM-Speicher bereit
 */

#ifndef SRC_BA_EEPROM_H_
#define SRC_BA_EEPROM_H_
#include<stdint.h>
#include"main.h"
#include"Instruction.h"


/**
  * @brief Liest eine Instruction aus dem EEPROM-Speicher
  * @param position Die Position der Instruction im Speicher
  * @return Gelesene Insruction
  */
Instruction getInstruction(int position);


/**
  * @brief Liest die Funktionsnummer einer Instruction and einer gegebenen Position im EEPROM
  * @param position Die Position der Instruction, von der die Funktionsnummer bestimmt werden soll
  * @return Die Funktionsnummer der Instruction
  */
uint8_t getFunctionNumberByPosition(int position);


/**
  * @brief Schreibt eine Instruction an gegebener Position in den EEPROM-Speicher
  * @param in Die Instruction, die geschrieben werden soll
  * @param position Die Position, an der die Instruction geschrieben werden soll
  */
void putInstruction(Instruction in, int position);


/**
  * @brief Löscht den gesamten EEPROM-Speicher
  */
void EEPROM_eraseAll();


#endif
