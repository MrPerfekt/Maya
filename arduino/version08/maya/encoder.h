#ifndef KNOB_ENCODER
#define KNOB_ENCODER

#include "Arduino.h"
#include "positionerCounter.h"

class Encoder : public PositionerCounter {
  const uint8_t *pins;
  uint8_t pinStatus[2];
  
  public:
  static const int8_t encoderFsm[4][4];
  static uint8_t encoderCount;
  static const uint8_t maxEncoderCount = 4;
  static Encoder *allEncoder[maxEncoderCount];
  
  Encoder(const uint8_t pin[2], void (*notifyFunction)());
  void init(); //Arduino overrides interupts and pins bevore main routine!!
  void update();
};

#endif
