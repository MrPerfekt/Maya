#ifndef STEPPER
#define STEPPER

#include "Arduino.h"

class Stepper {
  static const uint8_t pinCnt = 4;//A+,A-,B+,B-
  static const uint8_t stateCnt = 8;
  const uint8_t fsm[stateCnt][pinCnt] = {
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
  volatile int32_t pos = 0;
  volatile int32_t posDelta = 0;
  volatile int32_t posReal = 0;
  
  Stepper(const uint8_t pins[pinCnt]);
  void init(); //Arduino overrides interupts and pins bevore main routine!!
  void stepAndUpdate();
};

#endif
