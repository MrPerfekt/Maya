#include "stepper.h"
#include "Arduino.h"

Stepper::Stepper(const uint8_t pins[pinCnt])
  : pins(pins) {
}

void Stepper::init() {
  for (int i = 0; i < pinCnt; i++) {
    pinMode(pins[i], OUTPUT);
  }
}

void Stepper::stepAndUpdate() {
  int32_t curPos = pos + posDelta;
  if (curPos > posReal) {
    posReal++;
  } else if (curPos < posReal) {
    posReal--;
  } else {
    //for(int i = 0; i < pinCnt; i++){
    //  digitalWrite(pins[stepper][i],LOW);
    //}
    return;
  }
  int state = posReal % stateCnt;
  if (state < 0) {
    state += stateCnt;
  }
  for (int i = 0; i < pinCnt; i++) {
    digitalWrite(pins[i], fsm[state][i]);
  }
}
