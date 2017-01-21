#ifndef DEBUG
#define DEBUG 1

#include "Arduino.h"

//==DebugPin==
const uint8_t debugPin = A6;

#define DEBUG_OUT(x) digitalWrite(debugPin, x);

#define DEBUG_A 0

#define MAIN_LOOP_TIMING 1

// TC1 0 TC3_IRQn    Stepper 0,1
#define TIMER_INTERRUPT_TIMING_A 0
// TC1 1 TC4_IRQn    Stepper 2
#define TIMER_INTERRUPT_TIMING_B 0
// TC1 2 TC5_IRQn    InterruptLaserDetector
#define TIMER_INTERRUPT_TIMING_C 0
// TC1 2 TC5_IRQn    InterruptLaserDetector First
#define TIMER_INTERRUPT_TIMING_D 0
// TC1 2 TC5_IRQn    InterruptLaserDetector Second
#define TIMER_INTERRUPT_TIMING_E 0
// TC2 0 TC6_IRQn    FakeCounter
#define TIMER_INTERRUPT_TIMING_F 0

// Counter
#define PIN_INTERRUPT_TIMING_A 0
// Encoder
#define PIN_INTERRUPT_TIMING_B 0
// StepperWatcher
#define PIN_INTERRUPT_TIMING_C 0
#define PIN_INTERRUPT_TIMING_D 0
#define PIN_INTERRUPT_TIMING_E 0

#if(DEBUG_A && DEBUG)
#define DEBUG_OUT_A(x) DEBUG_OUT(x);
#else
#define DEBUG_OUT_A(x)
#endif

#if(MAIN_LOOP_TIMING == 1 && DEBUG)
#define MAIN_LOOP_TIMING_WRITE_A(x) DEBUG_OUT(x);
#else
#define MAIN_LOOP_TIMING_WRITE_A(x)
#endif
#if(TIMER_INTERRUPT_TIMING_A == 1 && DEBUG)
#define TIMER_INTERRUPT_TIMING_WRITE_A(x) DEBUG_OUT(x);
#else
#define TIMER_INTERRUPT_TIMING_WRITE_A(x)
#endif
#if(TIMER_INTERRUPT_TIMING_B == 1 && DEBUG)
#define TIMER_INTERRUPT_TIMING_WRITE_B(x) DEBUG_OUT(x);
#else
#define TIMER_INTERRUPT_TIMING_WRITE_B(x)
#endif
#if(TIMER_INTERRUPT_TIMING_C == 1 && DEBUG)
#define TIMER_INTERRUPT_TIMING_WRITE_C(x) DEBUG_OUT(x);
#else
#define TIMER_INTERRUPT_TIMING_WRITE_C(x)
#endif
#if(TIMER_INTERRUPT_TIMING_D == 1 && DEBUG)
#define TIMER_INTERRUPT_TIMING_WRITE_D(x) DEBUG_OUT(x);
#else
#define TIMER_INTERRUPT_TIMING_WRITE_D(x)
#endif
#if(TIMER_INTERRUPT_TIMING_E == 1 && DEBUG)
#define TIMER_INTERRUPT_TIMING_WRITE_E(x) DEBUG_OUT(x);
#else
#define TIMER_INTERRUPT_TIMING_WRITE_E(x)
#endif
#if(TIMER_INTERRUPT_TIMING_F == 1 && DEBUG)
#define TIMER_INTERRUPT_TIMING_WRITE_F(x) DEBUG_OUT(x);
#else
#define TIMER_INTERRUPT_TIMING_WRITE_F(x)
#endif


#if(PIN_INTERRUPT_TIMING_A == 1 && DEBUG)
#define PIN_INTERRUPT_TIMING_WRITE_A(x) DEBUG_OUT(x);
#else
#define PIN_INTERRUPT_TIMING_WRITE_A(x)
#endif
#if(PIN_INTERRUPT_TIMING_B == 1 && DEBUG)
#define PIN_INTERRUPT_TIMING_WRITE_B(x) DEBUG_OUT(x);
#else
#define PIN_INTERRUPT_TIMING_WRITE_B(x)
#endif
#if(PIN_INTERRUPT_TIMING_C == 1 && DEBUG)
#define PIN_INTERRUPT_TIMING_WRITE_C(x) DEBUG_OUT(x);
#else
#define PIN_INTERRUPT_TIMING_WRITE_C(x)
#endif
#if(PIN_INTERRUPT_TIMING_D == 1 && DEBUG)
#define PIN_INTERRUPT_TIMING_WRITE_D(x) DEBUG_OUT(x);
#else
#define PIN_INTERRUPT_TIMING_WRITE_D(x)
#endif
#if(PIN_INTERRUPT_TIMING_E == 1 && DEBUG)
#define PIN_INTERRUPT_TIMING_WRITE_E(x) DEBUG_OUT(x);
#else
#define PIN_INTERRUPT_TIMING_WRITE_E(x)
#endif

#endif
