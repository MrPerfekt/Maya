#ifndef KNOB_ENCODER
#define KNOB_ENCODER

#include "Arduino.h"

extern uint8_t *knobEncoderPos;
void setKnobEncoders(uint8_t count, uint8_t pin[][3], void (*notifyFunctions[][2])());//pinButton, pinA, pinB

#endif
