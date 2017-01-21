#ifndef COUNTER
#define COUNTER

#include "Arduino.h"
#include "positionerCounter.h"

class Counter : public PositionerCounter {
  uint8_t pin;
  uint8_t pinStatus;
  
  public:
  static uint8_t counterCount;
  static const uint8_t maxCounterCount = 1;
  static Counter *allCounter[maxCounterCount];
  
  Counter(uint8_t pin, void (*notifyFunction)());
  void init();
  void update();
};

#endif
