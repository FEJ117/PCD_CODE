
 /** @example LD1/LD2 This instruction will set the left (LD1) or right (LD2) LED to a chosen colour, where:
 R = Red, G = Green, B = Blue, V = Violett, O = Orange, T = Turquoise, W = White, other = OFF. \n
 The code: \n
 LD1  V \n
 LD2  B \n
 will set the left LED to violett and the right one to blue.
 */

/** @example WAI This instruction waits a certain amount of time (in 1/10 seconds) before continuing the program. \n
The code \n
LD1 V \n
WAI 5 \n
LD1 A \n
WAI 5 \n
JUM 0 \n
will make the left LED blink violett with a frequency of 1 Hz.
*/
 
/** @example TON This instruction plays a tone given in the format C#7, here C is any tone (C, D, E, F, G, A, H),
7 is the pitch (choose from 1-7) and # indicates the semitone higher then C. # is optional. TON 0 will turn off the speaker. \n
The code: \n
TON H4 \n
WAI 10 \n
TON G4 \n
WAI 10 \n
TON A4 \n
WAI 10 \n
TON D4 \n
WAI 10 \n
TON 0  \n
will play the westminster chime.
*/

/** @example SET This instruction sets the register chosen by the instruction PIC to a desired value.
Please refer to the example of PIC.
*/

/** @example ADD/SUB This instructions adds a register to the register chosen by the instruction PIC.
Please refer to the example of PIC.
*/

/** @example PIC The code: \n
PIC R0  \n
SET 5   \n
PIC R1  \n
SET 3   \n
ADD R0  \n
PTR R1  \n
WAI 100 \n
is a very complicated way for the display to show the number 8.
*/

/** @example Conditions This example will show the way all conditions work.
Once the program determines if the condition is true, 
it will jump into the program block specified by BEG and END if the condition is true and
jump behind END if the condition is false. \n
The code: \n
CONDITION \n
BEG       \n
LD1 V     \n
JUM 0     \n
END       \n
LD1 A     \n
JUM 0     \n
will set the left LED to violett if the condition is true and to OFF if the condition is false.
*/

/** @example BEG/END The instructions BEG and END are used to specify program blocks. \n
The code: \n
INH 0   \n
BEG     \n
LD1 V   \n
WAI 5   \n
LD1 A   \n
WAI 5   \n
END     \n
JUM 0   \n
will let the left LED blink when the first Button is pressed. Everything between BEG and END
is only being executed, if the condition (INH 0) is true.
*/

/** @example SMA/BIG These instructions are a condition which determines, if the value of the register chosen by the instruction PIC
is smaller or bigger then the value of the register specified. The instruction SMA R1 will be true, if the value of the
register chosen by the instruction PIC is smaller than the value of register 1. \n
As an example please insert: \n
PIC R0 \n
SET 10 \n
SMA R1 \n
into the exaple "Conditions". The LED will remain OFF. If you switch the condition to BIG R1, the LED will turn on.
*/

/** @example REQ/RNQ These work the same as SMA/BIG but will determine if the two registers are the same*/

/** @example VEQ/VNQ These work the same as SMA/BIG but will determine if value of the register chosen by the instruction PIC
is the same as a given number.
*/

/** @example INH/INL These conditions will be true if the given input is HIGH (INH) or LOW (INL).
If used with the example "Conditions" and the condition INH 0, the program will turn on the LED, if the
left button is pressed. \n
0 = Button1 \n
1 = Button2 \n
2 = Button3 \n
3 = Button4
*/

/** @example ANH/ANL These conditions determine if the value of the analog input given in the instruction is
higher (ANH) or lower (ANL) as the value of the register chosen by the instruction PIC.
Used with the example "Conditions" and the code: \n
PIC R0  \n
SET 110 \n
ANH 8   \n
the program will turn on the LED if the brightness sensor outputs a value higher then 110.
*/

/** @example SVA This instruction saves the value of an analog input to the register chosen by the instruction PIC. \n
The code: \n
PIC R0  \n
SVA 8   \n
PTR R0  \n
WAI 5   \n
CLR     \n
JUM 0   \n
will print out the analog value of the brightness sensor.
*/


/** @example COP This instruction will copy the value of the register selected by the instruction PIC to another register. \n
The code: \n
PIC R0    \n
SET 5     \n
COP R1    \n
PTR R1    \n
WAI 100   \n
will copy the value of register 0 to register 1 making the value of register 1 5 as well.
*/

/** @example INC/DEC This instruction increments or decrements the register chosen by the instruction PIC. \n
The code: \n
PIC R1  \n
SET 0   \n
INC 8   \n
DEC 3   \n
PTR R1  \n
WAI 100 \n
will output the number 5 to the screen since the Register (R1) is first set to 0, then incremented by 8 and then decremented by 3.
Setting the register to 0 is not strictly necessary here, since all regiters are always 0 at the beginning of the program.
*/


/** @example PTR This instruction writes the value of the given Register to the screen.
Please refer to the example of PIC
*/

/** @example PCH This instruction writes any three characters to the screen. Characters that arent specified are ignored. \n
The code: \n
PCH HEL \n
PCH LO  \n
WAI 100 \n
will print "Hello" to the screen.
*/

/** @example CLR This instruction clears the whole screen. No data needed.
*/

/** @example JUM This instruction will jump to a specifies place in the program. Please refer to the example of WAI.
*/



/** @example SPO/JPO These instructions help with sub-programs by allowing to store addresses in registers.
SPO will save the current position into the register chosen by the instruction PIC. JPO will jump to the position chosen by the function PIC. \n
The code: \n
SPO R0  \n
LD1 V   \n
WAI 5   \n
LD1 A   \n
WAI 5   \n
JPO R0  \n
will let the left LED blink violett. It does not matter if there is code inserted above SPO.
 @warning As of now these only work with position less then 256.
*/

