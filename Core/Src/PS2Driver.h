/**
 * @file PS2Driver.h
 * @brief Provides function to read input from the PS/2 keyboard to other functions
 */

#ifndef SRC_PS2DRIVER_H_
#define SRC_PS2DRIVER_H_

typedef enum{
	KEY_BACKSPACE = 128,
	KEY_DEL,
	KEY_ENTER,
	KEY_SMALLER,
	KEY_BIGGER,
	KEY_EQUAL,
	KEY_UP,
	KEY_DOWN,
	KEY_INSERT,
	KEY_DELETE
 } ps2Key_t;

/**
 * @brief Stores the bits last received by the keyboard
 */
extern long keyBuffer;

/**
 * @brief Stores the current position in the PS/2 dataframe
 */
extern char framePos;

/**
  * @brief  Returns the key last pressed on the keyboard and empties the buffer
  * @retval Key that has been pressed (not all keys supported. Unsupported keys result in return 0)
  */
char PS2_GetKey();



#endif /* SRC_PS2DRIVER_H_ */
