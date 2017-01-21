#ifndef TIMER
#define TIMER

#include "Arduino.h"

void setTimerSpeed(Tc *tc, uint32_t channel, uint32_t frequency, uint8_t divider);
void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency, uint8_t divider);
void setTimerSpeed(Tc *tc, uint32_t channel, uint32_t frequency);
void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency);

#endif

/*
ISR/IRQ		TC		Channel		Due pins
TC0			TC0		0			2, 13
TC1			TC0		1			60, 61
TC2			TC0		2			58
TC3			TC1		0			none
TC4			TC1		1			none
TC5			TC1		2			none
TC6			TC2		0			4, 5
TC7			TC2		1			3, 10
TC8			TC2		2			11, 12

TCn:
Timer counter module (3x)
Timer counter channel (9x, 3 per module).
*/
