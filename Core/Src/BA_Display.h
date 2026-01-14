/**
 * @file BA_Display.h
 * @brief Provides functions for the use of the OLED-display to other files
 */


#ifndef BA_DISPLAY_H
#define	BA_DISPLAY_H
#include<stdint.h>


/**
  * @brief Sets position of the cursor
  * @param x Horizontal position of the cursor
  * @param page Selected line
  */
void SetCursor(uint8_t x, uint8_t page);



/**
  * @brief Sets the contrast of the Display
  * @param value Chosen contrast
  */
void setContrast(uint8_t value);


/**
  * @brief Initializes the display
  * @warning Must be run in order for the Display to work
  */
void SSDINIT();


/**
  * @brief Sets all pixels to black
  */
void FillBlack(void);


/**
  * @brief Writes a left-facing arrow on the right side of the chosen line
  * @param line Line in which to write the arrow
  */
void writeLeftArrow(uint8_t line);

/**
  * @brief Writes a string of characters onto the display
  * @details Automatically breaks the line if necessary
  * @param str[] String of data to be written
  * @param size Size of str[] in bytes
  * @param pos Horizontal position of the first character
  * @param line Line of the first character
  */
void Display_WriteString(char str[], uint8_t size, uint8_t pos, uint8_t line);


/**
  * @brief Writes a single character onto the display
  * @param ch Character to be written
  * @param pos Horizontal position of the character
  * @param line Line of the character
  */
void Display_WriteCharacter(char ch, uint8_t pos, uint8_t line);


/**
  * @brief Shows a message to tell the user the device has been switched into programming mode
  * @details Is automatically called everytime the mode switches to programming mode
  */
void showProgrammingModeMessage();


/**
  * @brief Shows a message to tell the user the device has been switched into execution mode
  * @details Is automatically called everytime the mode switches into execution mode
  */
void showExecutionModeMessage();


/**
  * @brief Shows a message to tell the user the program has been terminated
  * @details Is automatically called once the program is terminated
  */
void showProgramTerminatedMessage();

/**
  * @brief 	Shows a message stating that an error has occured and provides the line of code the error occured in
  * @param 	line Line of Code the error occured in
  */
void fehlerMeldung(int line);



#endif

