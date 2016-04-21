#ifndef _ACROBOT_PIN_CHANGE_H
#define _ACROBOT_PIN_CHANGE_H

#include "Utils.h"

namespace ACRobot {
/*
   an extension to the interrupt support for arduino.
   add pin change interrupts to the external interrupts, giving a way
   for users to have interrupts drive off of any pin.
   Refer to avr-gcc header files, arduino source and atmega datasheet.
*/

/*
   Theory: all IO pins on Atmega168 are covered by Pin Change Interrupts.
   The PCINT corresponding to the pin must be enabled and masked, and
   an ISR routine provided.  Since PCINTs are per port, not per pin, the ISR
   must use some logic to actually implement a per-pin interrupt service.
*/

/* Pin to interrupt map:
   D0-D7 = PCINT 16-23 = PCIR2 = PD = PCIE2 = pcmsk2
   D8-D13 = PCINT 0-5 = PCIR0 = PB = PCIE0 = pcmsk0
   A0-A5 (D14-D19) = PCINT 8-13 = PCIR1 = PC = PCIE1 = pcmsk1
*/

typedef void (*voidFuncPtr)(void);

/*
   attach an interrupt to a specific pin using pin change interrupts.
*/
void PCattachInterrupt(uint8_t pin, void (*userFunc)(void), int mode);
void PCdetachInterrupt(uint8_t pin);

} // ACRobot namespace

#endif

