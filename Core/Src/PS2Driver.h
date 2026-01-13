/**
 * @file PS2Driver.h
 * @brief Stellt Funktionen zur Benutzung einer PS/2 Tastatur zur Verfügung
 */

#ifndef SRC_PS2DRIVER_H_
#define SRC_PS2DRIVER_H_

/**
 * @brief Speichert die zuletzt über den PS/2-Bus empfangenen Bits
 */
extern long keyBuffer;

/**
 * @brief Speichert die aktuelle Position im PS/2 Frame
 */
extern char framePos;

/**
  * @brief  Gibt die letzte Taste auf der Tastatur aus, die gedrückt wurde (nur ausgewählte Tasten) und leert den Buffer
  * @retval Taste, die gedrückt wurde als Zeichen
  */
char getKey();



#endif /* SRC_PS2DRIVER_H_ */
