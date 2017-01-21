#ifndef FAKE_COUNTER
#define FAKE_COUNTER

#include "Arduino.h"
#include "positionerCounter.h"

class FakeCounter : public PositionerCounter {
  public:
	  FakeCounter(void (*notifyFunction)());
  void init();
  void update();
};

#endif
