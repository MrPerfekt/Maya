#ifndef TIMER
#define TIMER

#include "Arduino.h"

void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency);

#endif
