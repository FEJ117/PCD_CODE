[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://FEJ117.github.io/PCD_CODE/)

# Programmlogik
Die Dateien InstructionList.h, InstructionList.c und Instruction.h enthalten die eigentliche Struktur des Programms. Sie enthalten die möglichen Befehle, sowie die Art und Weise in der die Befehle eingegeben und ausgeführt werden. Die Dateien enthalten keine Funktionen, die STM32-Spezifisch sind. Das bedeutet, dass diese Dateien genau so in einem Projekt auf einer anderen Mikroprozessorplattform (Beispielsweise ATMega oder PIC) eingesetzt werden können.

# STM32-Spezifische Programmelemente
Im Gegensatz zu den in "Programmlogik" genannten Dateien enthalten die übrigen Dateien Funktionen, die nur im STM32-Entwicklungsumfeld eingesetzt werden können. Dazu gehören die Steuerung des Displays (BA_Display.c und BA_Display.h), das Auslesen der Tastatur (PS2Driver.c und PS2Driver.h), das Schreiben und Auslesen des EEPROM-Speichers (BA_EEPROM.c und BA_EEPROM.h), sowie weitere Funktionen, beispielsweise zur Steuerung der GPIO-Ports (STM_FUNCTIONS.c und STM_FUNCTIONS.h).
Bei einem Wechsel auf eine andere Plattform muss davon ausgegangen werden, dass die in den Dateien enthaltenen Funktionen durch Funktionen ersetzt werden müssen, die mit der neuen Plattform kompatibel sind. Die Headerdateien (.h) geben dabei jeweils an, auf welche der Funktionen von Außen, das heißt innerhalb einer anderen Datei, zugegriffen wird. Die in den Headerdateien angegebenen Funktionen müssen bei einem Wechsel der Plattform zwingend zur Verfügung gestellt werden.
