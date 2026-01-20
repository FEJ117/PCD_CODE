# Full documentation can be accessed here: [![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://FEJ117.github.io/PCD_CODE/index.html)

# The programs main logic
The files InstructionList.h, InstructionList.c, Instructionhandlers.c, InstructionHandlers.h and Instruction.h contain the logical structure used to program the device and execute instructions. They contain all defined function identifiers and the way the functions are being called. They do not contain STM32-specific functions. This means they can be exported to other projects using (for example) ATmega or PIC.

# STM32-Specific elements
All other files contain functions specific to the STM32 environment. This includes the files for communicating with the OLED display (Display.c and Display.h), reading the keyboard (PS2Driver.c and PS2Driver.h), reading from and writing to the EEPROM (EEPROM.c and EEPROM.h) and all other functions (STM_FUNCTIONS.c and STM_FUNCTIONS.h).
When switching platforms, make sure to replace all STM32-specific functions with those suitable for your platform. The headers (.h) contain all functions accessed by outside functions in other files. Make sure to provide all of those functions.

# Adding commands to the system
To add a command, please follow the instructions provided in the documentation of InstructionHandlers.h.

