#ifndef KNOB_ENCODER
#define KNOB_ENCODER

#include "Arduino.h"

class Encoder{
  uint8_t *pins;
  uint8_t pinStatus[2];
  void (*notifyFunction)();
  
  public:
  static const int8_t encoderFsm[4][4];
  static uint8_t encoderCount;
  static const uint8_t maxEncoderCount = 2;
  static Encoder *allEncoder[maxEncoderCount];
  
  int32_t pos = 0;
  Encoder(uint8_t pin[2], void (*notifyFunction)());
  void init(); //Arduino overrides interupts and pins bevore main routine!!
  void update();
};

#endif
