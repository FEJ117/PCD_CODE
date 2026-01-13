/**
 * @file BA_Display.h
 * @brief Stellt Funktionen zur Benutzung des OLED-Displays zur Verfügung
 */


#ifndef BA_DISPLAY_H
#define	BA_DISPLAY_H
#include<stdint.h>


/**
  * @brief Setzt den Cursor des Displays an die gegebene Stelle
  * @param x Horizontale Position des Cursors
  * @param page Auszuwählende Zeile
  */
void SetCursor(uint8_t x, uint8_t page);



/**
  * @brief Setzt den Kontrast des Displays
  * @param value Gewählter Kontrast
  */
void setContrast(uint8_t value);


/**
  * @brief Initialisiert das Display
  */
void SSDINIT();


/**
  * @brief Setzt alle Pixel des Displays auf schwarz
  */
void FillBlack(void);


/**
  * @brief Schreibt in der gegebenen Zeile einen nach links zeigenden Pfeil an die rechte Seite des Displays
  * @param line Zeile, in die der Pfeil geschrieben werden soll
  */
void writeLeftArrow(uint8_t line);

/**
  * @brief Schreibt eine Zeichenfolge an die gewünschte Stelle im Display (automatischer Zeilenumbruch bei Überschreiten des Displaybereichs)
  * @param str[] Zeichenfolge, die geschrieben werden soll
  * @param size Größe von str[] in Bytes
  * @param pos Die horizontale Position, an die die Zeichen geschrieben werden sollen
  * @param line Die Zeile, in die die Zeichen geschrieben werden sollen
  */
void Display_WriteString(char str[], uint8_t size, uint8_t pos, uint8_t line);


/**
  * @brief Schreibt ein einzelnes Zeichen an die gegebene Position
  * @param ch Das Zeichen, das geschrieben werden soll
  * @param pos Die horizontale Position, an die das Zeichen geschrieben werden soll
  * @param line Die Zeile, in die das Zeichen geschrieben werden soll
  */
void Display_WriteCharacter(char ch, uint8_t pos, uint8_t line);


/**
  * @brief 	Zeigt auf dem Display an, dass in den Programmiermodus gewechselt wurde.
  * 		Wird bei einem Wechsel automatisch aufgerufen.
  */
void showProgrammingModeMessage();


/**
  * @brief 	Zeigt auf dem Display an, dass in den Ausführen-Modus gewechselt wurde.
  * 		Wird bei einem Wechsel automatisch aufgerufen.
  */
void showExecutionModeMessage();


/**
  * @brief 	Zeigt auf dem Display an, dass das Programm beendet wurde.
  */
void showProgramTerminatedMessage();

/**
  * @brief 	Zeigt auf dem Display an, dass ein Fehler aufgetreten ist.
  * 		Gibt die Zeile an, in der der Fehler erkannt wurde.
  * @param 	line Zeile, in der der Fehler aufgetreten ist
  */
void fehlerMeldung(int line);



#endif

