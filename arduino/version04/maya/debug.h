#ifndef DEBUG
#define DEBUG 1

#include "Arduino.h"

//==DebugPin==
const uint8_t debugPin = DAC0;

#define DEBUG_OUT(x) digitalWrite(debugPin, x);

#define INTERRUPT_TIMING_A 0
#define INTERRUPT_TIMING_B 0
#define INTERRUPT_TIMING_C 0
#define INTERRUPT_TIMING_D 0
#define INTERRUPT_TIMING_E 1

#if(INTERRUPT_TIMING_A == 1 && DEBUG)
#define INTERRUPT_TIMING_WRITE_A(x) digitalWrite(debugPin, x);
#else
#define INTERRUPT_TIMING_WRITE_A(x)
#endif
#if(INTERRUPT_TIMING_B == 1 && DEBUG)
#define INTERRUPT_TIMING_WRITE_B(x) digitalWrite(debugPin, x);
#else
#define INTERRUPT_TIMING_WRITE_B(x)
#endif
#if(INTERRUPT_TIMING_C == 1 && DEBUG)
#define INTERRUPT_TIMING_WRITE_C(x) digitalWrite(debugPin, x);
#else
#define INTERRUPT_TIMING_WRITE_C(x)
#endif
#if(INTERRUPT_TIMING_D == 1 && DEBUG)
#define INTERRUPT_TIMING_WRITE_D(x) digitalWrite(debugPin, x);
#else
#define INTERRUPT_TIMING_WRITE_D(x)
#endif
#if(INTERRUPT_TIMING_E == 1 && DEBUG)
#define INTERRUPT_TIMING_WRITE_E(x) digitalWrite(debugPin, x);
#else
#define INTERRUPT_TIMING_WRITE_E(x)
#endif

#endif
