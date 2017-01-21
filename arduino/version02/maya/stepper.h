#ifndef STEPPER
#define STEPPER

#include "Arduino.h"

class Stepper {
  static const uint8_t stepperStatePinCnt = 4;
  static const uint8_t stepperPinCnt = 5;//A+,A-,B+,B-,PWM
  static const uint8_t stepperStateCnt = 8;
  const uint8_t fsmStepper[stepperStateCnt][stepperStatePinCnt] = {
    {1,0,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,1,1},
    {0,0,0,1},
    {1,0,0,1}
  };
  
  const uint8_t *pins;
  
  public:
  volatile int32_t stepperPos;
  volatile int32_t stepperPosDelta;
  volatile int32_t stepperPosReal;
  
  Stepper(const uint8_t stepperPins[stepperPinCnt]);
  void stepAndUpdate();
};

#endif
