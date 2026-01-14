/**
 * @file STM_FUNCTIONS.h
 * @brief Provides STM-specific functions to other files
 */

#ifndef STM_FUNCTIONS_H
#define STM_FUNCTIONS_H

#include <stdbool.h>
#include "BA_Display.h"
#include "BA_EEPROM.h"
#include "InstructionList.h"
#include "Instruction.h"

/**
 * @brief Determines if the first button is being pressed
 */
#define isButton1Pressed() (HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin) == GPIO_PIN_RESET)

/**
 * @brief Determines if the second button is being pressed
 */
#define isButton2Pressed() (HAL_GPIO_ReadPin(Button2_GPIO_Port, Button2_Pin) == GPIO_PIN_RESET)

/**
 * @brief Determines if the third button is being pressed
 */
#define isButton3Pressed() (HAL_GPIO_ReadPin(Button3_GPIO_Port, Button3_Pin) == GPIO_PIN_RESET)

/**
 * @brief Determines if the fourth button is being pressed
 */
#define isButton4Pressed() (HAL_GPIO_ReadPin(Button4_GPIO_Port, Button4_Pin) == GPIO_PIN_RESET)

/**
 * @brief Determines if the mode switch is set to programming mode
 */
#define isProgrammingMode() HAL_GPIO_ReadPin(ModeSwitch_GPIO_Port, ModeSwitch_Pin)


/**
 * @brief Saves the system time of the last WAI instruction to allow for compensation of other functions runtime
 * @details (system time rolls over every 50 days so roll over protection is unnecessary)
 */
extern uint32_t lastTick;


/**
  * @brief Waits the given amount of time (in 1/10 seconds)
  * @param pData Delay time (in 1/10 seconds)
  */
void wait(uint16_t pData);


/**
  * @brief Sets the colour of one of the two RGB-LEDs
  *
  * @param fNumber Position of the LED: 0 = first, 1 = second
  *
  * @param colour Wanted colour: 'R' = red, 'G' = green, 'B' = blue,
  * 'O' = orange, 'V' = violet, 'T' = turquoise, 'W' = white, Everything else = off
  */
void setLED(uint8_t fNumber, char colour);

/**
  * @brief Activates the buzzer with the tone and pitch information provided by an instruction
  * @param ex Instruction containing the tone's information
  */
void activateBuzzer(Instruction ex);

/**
  * @brief Reads one of the 9 ADC-Channels
  * @param channel The chosen channel
  * @return The analog value of the channel
  */
int STM_readADC(uint8_t channel);

/**
  * @brief Determines, if a certain input is high
  * @param port The input to be processed: 0: Button 1, 1: Button 2, 2: Button 3, 3: Button 4
  * @return Bool that states if the input is high
  */
bool isInputHigh(uint8_t port);

/**
  * @brief Returns the chars corresponding to a number with 3 decimal places
  * @param pNum The number to be converted
  * @param pOut The array in which to write the corresponding characters
  */
void number3ToChar(uint16_t pNum, char pOut[3]);


#endif




