/**
 * @file Display.c
 * @brief Implementation of Display-Functions
 */

#include "Font.h"
#include "Display.h"
#include "STM_FUNCTIONS.h"
#include "main.h"
#include<stdint.h>

/**
  *	@brief I2C object created by HAL
  */
extern I2C_HandleTypeDef hi2c1;

/**
  *	@brief I2C-address of the display
  */
uint16_t addr = 0x3C;

/**
  * @brief Writes one or more commands to the display
  * @details Can be used to initialize the display, set the cursor or set brightness
  * @param pCommands[] List of commands
  * @param pSize Size of pCommands[] in bytes
  */
void SSD_WriteCommands(uint8_t pCommands[], uint16_t pSize)
{
	uint8_t buffer[2];
	buffer[0] = 0x00;  // Control byte for command
	for(uint16_t i = 0; i < pSize; i++)
	{
		buffer[1] = pCommands[i];
		HAL_I2C_Master_Transmit (&hi2c1, 0x3C << 1, buffer, sizeof(buffer), 10000);
	}
}


//Documented in .h
void SetCursor(uint8_t x, uint8_t page)
{
	uint8_t com[] = {(0xB0 | (page & 0x07)),(0x00 | (x & 0x0F)),(0x10 | ((x >> 4) & 0x0F))};
	SSD_WriteCommands(com, sizeof(com));
}

//Documented in .h
void setContrast(uint8_t value)
{
	uint8_t buffer[3];
	buffer[0] = 0x00;  // Control byte for command
	buffer[1] = 0x81;
	buffer[2] = value;
	HAL_I2C_Master_Transmit (&hi2c1, 0x3C << 1, buffer, sizeof(buffer), 10000);
}

/**
  * @brief Writes a byte of data to the display (The display automatically increments the cursor psoition)
  * @param data Byte of data that is sent to the Display
  */
void SSD1306_WriteData(uint8_t data)
{
    uint8_t buffer[2];
    buffer[0] = 0x40;  // Control byte for data
    buffer[1] = data;
    HAL_I2C_Master_Transmit (&hi2c1, 0x3C << 1, buffer, sizeof(buffer), 10000);
}

//Documented in .h
void SSDINIT()
{
	uint8_t initCommands[] =
	{
			0xAE,0xD5,0x80,0xA8,0x1F,0xD3,0x00,0x40,0x8D,0x14,
			0x20,0x00,0xA1,0xC8,0xDA,0x02,0x81,0x7F,0xD9,0xF1,
			0xDB,0x40,0xA4,0xA6,0xAF,0x20,0x00,0x21,0x00,0x7F,
			0x22,0x00,0x03
	};

	SSD_WriteCommands(initCommands, sizeof(initCommands));

    for (uint16_t i = 0; i < 512; i++)
    {
        SSD1306_WriteData(0x00);
    }

}

//Documented in .h
void FillBlack(void)
{
	uint8_t commands[]=
	{
			0x20,0x00,0x21,0x00,0x7F,0x22,0x00,0x03
	};

	SSD_WriteCommands(commands, sizeof(commands));

    for (uint16_t i = 0; i < 512; i++)
    {
        SSD1306_WriteData(0x00);
    }

}


/**
  *	@brief Structure of pixels of a left-facing arrow
  */
const char leftArrow[8] =
{
    0b00000000,
    0b00011000,
    0b00011000,
    0b00111100,
    0b00111100,
    0b01111110,
    0b01111110,
    0b11111111
};

//Documented in .h
void writeLeftArrow(uint8_t line)
{
    for(uint8_t j = 0; j < 4; j++)
    {
        SetCursor(112,j);
        for(uint8_t i = 0; i < 8; i++)
        {
            SSD1306_WriteData(0x00);
        }
    }
    SetCursor(112,line);
    for(uint8_t i = 0; i < 8; i++)
    {
        SSD1306_WriteData(leftArrow[i]);
    }
}


//Documented in .h
void Display_WriteCharacter(char ch, uint8_t pos, uint8_t line)
{
	if(ch == 0) ch = ' ';
    SetCursor(pos*8,line);
    for(uint16_t i = (ch-32)*8; i < (ch-31)*8; i++)
    {
        SSD1306_WriteData(display_font[i]);
    }

    SetCursor(pos*8,line+1);
    for(uint16_t i = (ch+63)*8; i < (ch+64)*8; i++)
    {
        SSD1306_WriteData(display_font[i]);
    }
}

//Documented in .h
void Display_WriteString(char str[], uint8_t size, uint8_t pos, uint8_t line)
{
	for(uint8_t j = 0; j < size; j++)
	{
		if(pos+j > 15)
		{
			line = line + 2;
			pos = pos - 16;
		}
		if(str[j] == 0) str[j] = ' ';

		Display_WriteCharacter(str[j],pos+j,line);
	}
}


//Documented in .h
void showProgrammingModeMessage()
{
	char progMsg[] =
	{
			'P','R','O','G','R','A','M','M','I','E','R','E','N'
	};

	FillBlack();
	Display_WriteString(progMsg, sizeof(progMsg), 2, 1);
	HAL_Delay(500);

	FillBlack();
}


//Documented in .h
void showExecutionModeMessage()
{
	char startMsg[] =
	{
			'S','T','A','R','T','E','N'
	};

	FillBlack();
	Display_WriteString(startMsg, sizeof(startMsg), 4, 1);
	HAL_Delay(500);

	FillBlack();
}


//Documented in .h
void showProgramTerminatedMessage()
{
	char letters1[] =
	{
			'P','R','O','G','R','A','M','M'
	};
	char letters2[] =
	{
			'B','E','E','N','D','E','T'
	};
    FillBlack();
    HAL_Delay(500);
    Display_WriteString(letters1, sizeof(letters1), 4, 0);
    Display_WriteString(letters2, sizeof(letters2), 4, 2);
    HAL_Delay(500);
}

//Documented in .h
void fehlerMeldung(int line)
{
	char numChars[3];
	number3ToChar(line, numChars);

	char letters1[] =
	{
			'F','E','H','L','E','R',' ','I','N'
	};
	char letters2[] =
	{
			'Z','E','I','L','E',' ',numChars[0],numChars[1],numChars[2]
	};

	FillBlack();
	Display_WriteString(letters1, sizeof(letters1), 3, 0);
    Display_WriteString(letters2, sizeof(letters2), 3, 2);
	while(!isProgrammingMode());
}



