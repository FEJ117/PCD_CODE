/**
 * @file STM_FUNCTIONS.c
 * @brief Inplementation of STM32-specific functions
 */

#include <stdbool.h>
#include <stdint.h>
#include "InstructionList.h"
#include "Display.h"
#include "EEPROM.h"
#include "STM_FUNCTIONS.h"
#include "main.h"

/**
 * @brief Frequency of the buzzer
 */
uint32_t frequency = 0;

//Documented in .h
uint32_t lastTick = 0;


/**
 * @brief Timer object used for the buzzer (provided by HAL)
 */
extern TIM_HandleTypeDef htim14;

/**
 * @brief ADC object for reading analog inputs (provided by HAL)
 */
extern ADC_HandleTypeDef hadc;


//Documented in .h
void wait(uint16_t pData)
{
    lastTick = lastTick+(pData*100);
    while(HAL_GetTick() < lastTick && !isProgrammingMode());
}

//Documented in .h
void setLED(uint8_t fNumber, char colour)
{
	char colourCode = 0b111;
	switch(colour)
	{
		case 'R':
			colourCode = 0b011;
			break;
		case 'G':
			colourCode = 0b101;
			break;
		case 'B':
			colourCode = 0b110;
			break;
		case 'O':
			colourCode = 0b001;
			break;
		case 'V':
			colourCode = 0b010;
			break;
		case 'T':
			colourCode = 0b100;
			break;
		case 'W':
			colourCode = 0b000;
			break;
	}

	if(fNumber == FUNCTION_LD1)
	{
		GPIOB->ODR = (GPIOB->ODR & 0b1111110001111111) | colourCode << 7;
	}
	else
	{
		GPIOB->ODR = (GPIOB->ODR & 0b1111111110001111) | colourCode << 4;
	}
}

/**
 * @brief Struct to save the frequency of a note together with the semitone above it
 */
typedef struct {
    char note;
    uint16_t freq;
    uint16_t sharp;
} NoteEntry;

/**
 * @brief All notes and their frequencies (all pitch 7)
 */
static const NoteEntry notes[] = {
    {'C', 20930, 22175},
    {'D', 23493, 24890},
    {'E', 26370, 27938},
    {'F', 27938, 29600},
    {'G', 31360, 33224},
    {'A', 35200, 37293},
    {'H', 39511, 41860},
};

//Documented in .h
void activateBuzzer(Instruction ex)
{

	if(ex.data == '0'||ex.data == 0)
	{
		HAL_TIM_Base_Stop(&htim14);
		return;
	}

	uint8_t pitch = 0;

	for(uint8_t i=0;i<7;i++) {
		if(notes[i].note == ex.data) {
			frequency = (ex.data2 == '#') ? notes[i].sharp : notes[i].freq;
			break;
		}
	}

	if(ex.data2 != '#' && ex.data2 > 48 && ex.data2 < 57)
	{
			pitch = ex.data2-48;
    }
	else if(ex.data3 > 48 && ex.data3 < 57)
	{
			pitch = ex.data3-48;
	}

    for(uint8_t i = 0; i < 7-pitch; i++)
    {
    	frequency = frequency/2;
    }

    uint32_t timer_freq = (uint32_t)((uint32_t)HAL_RCC_GetPCLK1Freq() / (uint32_t)(htim14.Init.Prescaler + 1));
    uint32_t new_period = (uint32_t)((uint32_t)timer_freq*10) / (uint32_t)(2 * frequency);

	HAL_TIM_Base_Stop(&htim14);


	__HAL_TIM_SET_AUTORELOAD(&htim14, new_period - 1);

	// Reset the counter to avoid glitches
	__HAL_TIM_SET_COUNTER(&htim14, 0);

	// Restart the timer
	HAL_TIM_Base_Start(&htim14);
}


//Documented in .h
int STM_readADC(uint8_t channel)
{

	uint8_t adc_value = 0;
	for(int i = 0; i < 9; i++)
	{
		HAL_ADC_Start(&hadc);
		HAL_ADC_PollForConversion(&hadc, 100);
		if(i == channel)
		adc_value = HAL_ADC_GetValue(&hadc);
	}

	return(adc_value);
}


//Documented in .h
bool isInputHigh(uint8_t port)
{
	HAL_Delay(10);
	switch(port)
	{
		case 0:
				return isButton1Pressed();
				break;
		case 1:
				return isButton2Pressed();
				break;
		case 2:
				return isButton3Pressed();
				break;
		case 3:
				return isButton4Pressed();
				break;
	}
    return false;
}


//Documented in .h
void number3ToChar(uint16_t pNum, char pOut[3])
{
	pOut[0] = 48;
	pOut[1] = 48;
	pOut[2] = 48;
	//Diese Schleifen sind notwendig, weil der Prozessor keine Division großer Zahlen unterstützt
	//Die Bibliothek für die Operation (udivsi_3) braucht sehr viel Speicherplatz
	while(pNum >= 100){pOut[0]++;pNum-=100;}
	while(pNum >= 10){pOut[1]++;pNum-=10;}
	while(pNum >= 1){pOut[2]++;pNum-=1;}
	return;
}





