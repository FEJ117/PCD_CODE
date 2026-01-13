/**
 * @file STM_FUNCTIONS.h
 * @brief Enthält alle STM-Spezifischen Funktionen für InstructionList.h
 */

#ifndef STM_FUNCTIONS_H
#define STM_FUNCTIONS_H

#include <stdbool.h>
#include "BA_Display.h"
#include "BA_EEPROM.h"
#include "InstructionList.h"
#include "Instruction.h"

/**
 * @brief Prüft, ob der erste Taster gedrückt ist
 */
#define isButton1Pressed() (HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin) == GPIO_PIN_RESET)

/**
 * @brief Prüft, ob der zweite Taster gedrückt ist
 */
#define isButton2Pressed() (HAL_GPIO_ReadPin(Button2_GPIO_Port, Button2_Pin) == GPIO_PIN_RESET)

/**
 * @brief Prüft, ob der dritte Taster gedrückt ist
 */
#define isButton3Pressed() (HAL_GPIO_ReadPin(Button3_GPIO_Port, Button3_Pin) == GPIO_PIN_RESET)

/**
 * @brief Prüft, ob der vierte Taster gedrückt ist
 */
#define isButton4Pressed() (HAL_GPIO_ReadPin(Button4_GPIO_Port, Button4_Pin) == GPIO_PIN_RESET)

/**
 * @brief Prüft, ob der Modusschalter auf "Programmieren" steht
 */
#define isProgrammingMode() HAL_GPIO_ReadPin(ModeSwitch_GPIO_Port, ModeSwitch_Pin)


/**
 * @brief Speichert die Systemzeit seit dem letzten WAI-Befehl. Wird zur Synchronisierung benutzt
 */
extern uint32_t lastTick;


/**
  * @brief Wartet um die gegebene Zeit in 1/10 Sekunden
  * @param pData Die Zeit, um die das Programm angehalten werden soll in 1/10 Sekunden
  */
void wait(uint16_t pData);


/**
  * @brief Setzt eine der beiden LEDs auf eine gegebene Farbe
  *
  * @param fNumber Position der LED: 0 = LED1 (links), 1 = LED2 (rechts)
  *
  * @param colour Gewünschte Farbe: 'R' = Rot, 'G' = Grün, 'B' = Blau,
  * 'O' = Orange, 'V' = Violett, 'T' = Türkis, 'W' = Weiß, 'A' = Aus
  */
void setLED(uint8_t fNumber, char colour);

/**
  * @brief Aktiviert den Lautsprecher anhand der in der gegebenen Instruction gespeicherten Noteninformation
  * @param ex Die Instruction, die die Note enthält
  */
void activateBuzzer(Instruction ex);

/**
  * @brief Liest einen der 9 ADC-Kanäle aus
  * @param channel Der gewünschte Kanal
  * @return Der analoge 8-Bit-Wert des Kanals
  */
int STM_readADC(uint8_t channel);

/**
  * @brief Prüft, ob ein gegebener Input logisch 1 ist
  * @param port Der Input, der geprüft werden soll: 0: Taster 1, 1: Taster 2, 2: Taster 3, 3: Taster 4
  * @return Wahrheitswert, der angibt, ob der Input logisch 1 ist
  */
bool isInputHigh(uint8_t port);

/**
  * @brief Gibt eine Zeichenfolge für eine dreistellige Zahl zurück.
  * @param pNum Die Zahl, die konvertiert werden soll
  * @param pOut Das Array, in das die Zeichen geschrieben werden sollen
  */
void number3ToChar(uint16_t pNum, char pOut[3]);


#endif




