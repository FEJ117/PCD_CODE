#include "BA_EEPROM.h"
#include "Instruction.h"
#include "PS2Driver.h"
#include "InstructionList.h"
#include "STM_FUNCTIONS.h"

//Dokumentation in .h
uint16_t ind = 0;

/**
 * @brief Die Werte der 100 Speicherregister
 */
uint16_t registers[100];

/**
 * @brief Speichert, welches Register durch den Befehl PIC ausgewählt wurde
 */
uint8_t regPointer = 0;

/**
 * @brief Position des Cursors während der Ausgabe von Zeichen durch PCH und PTR
 */
uint8_t cursPos = 0;

/**
 * @brief Leere Instruction (Wird beim Einfügen von Programmzeilen benötigt)
 */
Instruction emptyInstruction = {0,0,0,0};


/**
  * @brief Löscht die Instruction an der gegebenen Position
  * @param pPos Position der Instruction
  */
void removeInstruction(uint8_t pPos)
{
    for(int i = pPos; i < 0xFF0; i++)
    {
    	Instruction in = getInstruction(i+1);
    	putInstruction(in,i);
    	if(in.functionNumber == FUNCTION_EMPTY) break;
    }
}


/**
  * @brief Fügt eine Instruction an der gegebenen Position ein
  * @param pPos Position, an der die Instruction eingefügt werden soll
  */
void insertInstruction(uint8_t pPos)
{
	int firstEmpty = 0;
	while(getFunctionNumberByPosition(firstEmpty) != FUNCTION_EMPTY){firstEmpty++;}
    for(int i = firstEmpty; i > pPos; i--)
    {
    	Instruction in = getInstruction(i-1);
		putInstruction(in,i);
    }

    putInstruction(emptyInstruction, pPos);
}

/**
  * @brief Schreibt den Bezeichner der Funktion einer gegebenen Instruction in das gegebene char-Array
  * @param pIn die Instruction, die ausgewertet wird
  * @param arr Das Array, in das der Bezeichner geschrieben wird
  */
void GetFunctionName(Instruction pIn, char arr[3])
{
    for (uint16_t i = 0; i < sizeof(functionNames) / sizeof(FunctionNameEntry); i++) {
        if (pIn.functionNumber == functionNames[i].functionNumber) {
            arr[0] = functionNames[i].name[0];
            arr[1] = functionNames[i].name[1];
            arr[2] = functionNames[i].name[2];
            return;
        }
    }

    arr[0] = 'X'; arr[1] = 'X'; arr[2] = 'X';
}

/**
  * @brief Schreibt den Inhalt von zwei Instructions zusammen mit den dazugehörigen Zeilennummern auf das Display
  * @param i1 Erste Instruction, die abgebildet werden soll
  * @param i2 Zweite Instruction, die abgebildet werden soll
  * @param pIndex Index, an dem die Insructions liegen
  */
void writeInstructions(Instruction i1, Instruction i2, uint16_t pIndex)
{
	char functionName[3];
	char numChars[3];
	GetFunctionName(i1,functionName);
	number3ToChar(pIndex, numChars);
	char instChars[] =
	{
			numChars[0],numChars[1],numChars[2],' ',':',' ',
			functionName[0],functionName[1],functionName[2],' ',
			i1.data,i1.data2,i1.data3
	};

	GetFunctionName(i2,functionName);
	number3ToChar(pIndex+1, numChars);
	char instChars2[] =
	{
			numChars[0],numChars[1],numChars[2],' ',':',' ',
			functionName[0],functionName[1],functionName[2],' ',
			i2.data,i2.data2,i2.data3
	};

	Display_WriteString(instChars, sizeof(instChars), 0, 0);
    Display_WriteString(instChars2, sizeof(instChars2), 0, 2);
}

/**
  * @brief 	Schreibt die Instruction, auf die der Index zeigt, sowie die vorherige Instruction auf das Display
  * 		und makiert die aktuelle Instruction mit einem Pfeil.
  */
void updateInstructions()
{
	writeInstructions(getInstruction(ind-(ind != 0)),getInstruction(ind+1-(ind != 0)),ind-(ind != 0));
	writeLeftArrow(3*(ind != 0));
}

/**
  * @brief 	Prüft, ob eine Bedingung gegeben ist.
  * 		- Wird BEG und END benutzt:
  * 			# Springt in den Programmblock, wenn die Bedingung zutrifft
  * 			# Springt hinter den Programmblock, wenn die Bedingung nicht zutrifft
* 			- Wird BEG und END nicht benutzt:
* 				# Springt zum nächsten Befehl, wenn die Bedingung zutrifft
* 				# Springt zum übernächsten Befehl, wenn die Bedingung nicht zutrifft
  * @param condition Die Bedingung, die abgefragt wird
  */
void evaluate(uint8_t condition) {

	if(!condition)
	{
		uint8_t openedBrackets = 1;
		if (getFunctionNumberByPosition(ind) == FUNCTION_BEG) {
			while (getFunctionNumberByPosition(ind) != FUNCTION_END
					|| openedBrackets > 0) {
				ind++;
				if (getFunctionNumberByPosition(ind) == FUNCTION_BEG)
					openedBrackets++;
				if (getFunctionNumberByPosition(ind) == FUNCTION_END)
					openedBrackets--;
			}
		}
		ind++;
	}
}


/**
  * @brief Schreibt bis zu 3 Zeichen an die aktuelle Position des Cursors
  * @param str[3] Zeichen, die geschrieben werden sollen
  */
void writeCharsAtCursorPosition(char str[3])
{
	Display_WriteString(str, 3, cursPos, 0);
	for(uint8_t i = 0; i < 3; i++)
	{
		if(str[i] != 0)cursPos++;
	}
}


/**
  * @brief Gibt die Funktionsnummer zurück, die zu der gegebenen Zeichenfolge passt
  * @param chars[] Die Zeichenfolge, bestehend aus 3 Zeichen (zum Beispiel WAI)
  * @return Die Funktionsnummer, die zu der Zeichenfolge passt. Wird keine Funktion gefunden: 255
  */
uint8_t charsToFunctionNumber(char chars[3])
{
	for (uint16_t i = 0; i < sizeof(functionNames) / sizeof(FunctionNameEntry); i++)
	{
		if (chars[0] == functionNames[i].name[0]&&chars[1] == functionNames[i].name[1]&&chars[2] == functionNames[i].name[2])
		{
			return(functionNames[i].functionNumber);
		}
	}
	return(255);

}



/**
  * @brief Führt die Funktion der Instruction, auf die der Index Zeigt aus
  */
void executeInstruction() {
	Instruction exe = getInstruction(ind);
	ind++;
	if (exe.data == 'R') {
		uint8_t regNumber;
		if (exe.data3 >= 58 || exe.data3 <= 48)
			regNumber = exe.data2 - 48;
		else if (exe.data2 >= 58 || exe.data2 <= 48)
			regNumber = ((exe.data2 - 48) * 10 + exe.data3 - 48);

		switch (exe.functionNumber) {
		case FUNCTION_PIC:
			regPointer = regNumber;
			break;
		case FUNCTION_COP:
			registers[regNumber] = registers[regPointer];
			break;
		case FUNCTION_ADD:
			registers[regPointer] += registers[regNumber];
			break;
		case FUNCTION_SUB:
			registers[regPointer] -= registers[regNumber];
			break;
		case FUNCTION_SMA:
			evaluate(registers[regPointer] < registers[regNumber]);
			break;
		case FUNCTION_BIG:
			evaluate(registers[regPointer] > registers[regNumber]);
			break;
		case FUNCTION_REQ:
			evaluate(registers[regPointer] == registers[regNumber]);
			break;
		case FUNCTION_RNQ:
			evaluate(registers[regPointer] != registers[regNumber]);
			break;
		case FUNCTION_PTR:
			char str[3];
			number3ToChar(registers[regNumber], str);
			writeCharsAtCursorPosition(str);
			break;
		case FUNCTION_SPO:
			registers[regNumber] = ind-1;
			break;
		case FUNCTION_JPO:
			ind = registers[regNumber];
			break;
		default:
			fehlerMeldung(ind-1);
			break;
		}
	}
	else
	{
		uint8_t datNumber = 0;
		if (exe.data >= 48 && exe.data <= 58) {
			if (exe.data3 >= 48 && exe.data3 <= 58) {
				datNumber = ((exe.data - 48) * 100 + (exe.data2 - 48) * 10
						+ exe.data3 - 48);
			} else if (exe.data2 >= 48 && exe.data2 <= 58) {
				datNumber = ((exe.data - 48) * 10 + exe.data2 - 48);
			} else if (exe.data >= 48 && exe.data <= 58) {
				datNumber = (exe.data - 48);
			}
		}

		switch (exe.functionNumber) {
		case FUNCTION_SET:
			registers[regPointer] = datNumber;
			break;
		case FUNCTION_WAI:
			wait(datNumber);
			break;
		case FUNCTION_INC:
			registers[regPointer] = registers[regPointer] + datNumber;
			break;
		case FUNCTION_DEC:
			registers[regPointer] = registers[regPointer] - datNumber;
			break;
		case FUNCTION_VEQ:
			evaluate(datNumber == registers[regPointer]);
			break;
		case FUNCTION_VNQ:
			evaluate(!(datNumber == registers[regPointer]));
			break;
		case FUNCTION_TON:
			activateBuzzer(exe);
			break;
		case FUNCTION_ANH:
			evaluate(registers[regPointer] < STM_readADC(datNumber));
			break;
		case FUNCTION_ANL:
			evaluate(!(registers[regPointer] < STM_readADC(datNumber)));
			break;
		case FUNCTION_SVA:
			registers[regPointer] = STM_readADC(datNumber);
			break;
		case FUNCTION_INH:
			evaluate(isInputHigh(datNumber));
			break;
		case FUNCTION_INL:
			evaluate(!(isInputHigh(datNumber)));
			break;
		case FUNCTION_JUM:
			ind = datNumber;
			break;
		case FUNCTION_LD1:
		case FUNCTION_LD2:
			setLED(exe.functionNumber, exe.data);
			break;
		case FUNCTION_PCH:
			char str[] = {exe.data, exe.data2, exe.data3};
			writeCharsAtCursorPosition(str);
			break;
		case FUNCTION_CLR:
			cursPos = 0;
			FillBlack();
			break;
		case FUNCTION_EMPTY:
			break;
		default:
			fehlerMeldung(ind-1);
			break;
		}
	}
}


//Dokumentation in .h
void init()
{
    for(uint16_t i = 0; i < 100; i++)
    {
    	registers[i] = 0;
    }
}


//Dokumentation in .h
void programmingMode(void)
{

    if(!(isProgrammingMode()))
        return;

    activateBuzzer(emptyInstruction);
    setLED(FUNCTION_LD1, 'A');
    setLED(FUNCTION_LD2, 'A');
    showProgrammingModeMessage();

    ind = 0;
    while((ind < 0x0FF0) && !(getFunctionNumberByPosition(ind) == FUNCTION_EMPTY))
        ind++;

    FillBlack();
    updateInstructions();

    char ch = 0;
    char linePos = 6;
    char instructionKeys[] = {0,0,0,0,0,0,0};

    while(isProgrammingMode())
    {
		while(ch == 0)
		{
			ch = getKey();
			if(!isProgrammingMode())
				return;
		}
		Instruction in;
		if(ch == ']')
		{
			if(instructionKeys[0] != 0)
			{
				char fName[] = {instructionKeys[0],instructionKeys[1],instructionKeys[2]};
				in.functionNumber = charsToFunctionNumber(fName);

				in.data = instructionKeys[4];
				in.data2 = instructionKeys[5];
				in.data3 = instructionKeys[6];
				putInstruction(in,ind);
			}
			ind++;
		}
		else if(ch == '^' && ind > 0)
		{
			ind--;
		}
		else if(ch == '.')
		{
			insertInstruction(ind);
		}
		else if(ch == ',')
		{
			removeInstruction(ind);
		}
		else if(ch == '<')
		{
			if(linePos > 6)
			{
				instructionKeys[linePos-7] = 0;
				Display_WriteCharacter(' ', --linePos,(ind != 0)*2);
			}
		}
		else
		{
			if(linePos == 6)
			for(int i = 0; i < 9; i++)
			{
				Display_WriteCharacter(' ',6+i,(ind!=0)*2);
			}
			if(linePos < 13)
			{
				instructionKeys[linePos-6] = ch;
				Display_WriteCharacter(ch, linePos++,(ind!=0)*2);
			}
		}

		if(ch == ']' || ch == '^' || ch == '.' || ch == ',')
		{
			updateInstructions();

			for(int i = 0; i < 7; i++)
			{
				instructionKeys[i] = 0;
			}

			linePos = 6;
		}

		keyBuffer = 0;
		framePos = 0;
		ch = 0;

    }

}

//Dokumentation in .h
void executionMode(void)
{
    if(isProgrammingMode())
        return;

    showExecutionModeMessage();

    for(uint8_t i = 0; i < 100; i++)
    {
    	registers[i] = 0;
    }
    ind = 0;
    regPointer = 0;
    cursPos = 0;

    lastTick = HAL_GetTick();
    while(!(isProgrammingMode()))
    {
        if(getFunctionNumberByPosition(ind) == FUNCTION_EMPTY || ind > 0xFF0)
            showProgramTerminatedMessage();
        else
            executeInstruction();
    }
}

