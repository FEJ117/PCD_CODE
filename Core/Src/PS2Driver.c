/**
 * @file PS2Driver.c
 * @brief Inplementation of function to use a PS/2 keyboard
 */

#include "PS2Driver.h"
#include "main.h"

//Documented in .h
char getKey()
{
	if(framePos < 11)
		return 0;

	char data = (keyBuffer >> 2) & 0b11111111;

	keyBuffer = 0;
	framePos = 0;

	data = ((data >> 1) & 0x55) | ((data << 1) & 0xAA);
    data = ((data >> 2) & 0x33) | ((data << 2) & 0xCC);
    data = ((data >> 4) & 0x0F) | ((data << 4) & 0xF0);


	switch(data)
	{
		case 0xF0: //Key released
					HAL_Delay(10);
					keyBuffer = 0;
					framePos = 0;
					return 0;
					break;

		case 0x5A:	//Enter
					return(']');
					break;

		case 0x75:	//Up
					return('^');
					break;

		case 0x70:	//Insert
					return('.');
					break;

		case 0x71:	//Delete
					return(',');
					break;

		case 0x66:	//Backspace
					return('<');
					break;

		case 0x29:	//Space
					return(' ');
					break;

		case 0x16:
					return('1');
					break;

		case 0x1E:
					return('2');
					break;

		case 0x26:
					return('3');
					break;

		case 0x25:
					return('4');
					break;

		case 0x2E:
					return('5');
					break;

		case 0x36:
					return('6');
					break;

		case 0x3D:
					return('7');
					break;

		case 0x3E:
					return('8');
					break;

		case 0x46:
					return('9');
					break;

		case 0x45:
					return('0');
					break;

		case 0x15:
					return('Q');
					break;

		case 0x1D:
					return('W');
					break;

		case 0x24:
					return('E');
					break;

		case 0x2D:
					return('R');
					break;

		case 0x2C:
					return('T');
					break;

		case 0x35:
					return('Z');
					break;

		case 0x3C:
					return('U');
					break;

		case 0x43:
					return('I');
					break;

		case 0x44:
					return('O');
					break;

		case 0x4D:
					return('P');
					break;

		case 0x1C:
					return('A');
					break;

		case 0x1B:
					return('S');
					break;

		case 0x23:
					return('D');
					break;

		case 0x2B:
					return('F');
					break;

		case 0x34:
					return('G');
					break;

		case 0x33:
					return('H');
					break;

		case 0x3B:
					return('J');
					break;

		case 0x42:
					return('K');
					break;

		case 0x4B:
					return('L');
					break;

		case 0x1A:
					return('Y');
					break;

		case 0x22:
					return('X');
					break;

		case 0x21:
					return('C');
					break;

		case 0x2A:
					return('V');
					break;

		case 0x32:
					return('B');
					break;

		case 0x31:
					return('N');
					break;

		case 0x3A:
					return('M');
					break;

		case 0x5D:
					return('#');
					break;


	}
	return(0);

}

