#include "stepper.h"
#include "Arduino.h"

Stepper::Stepper(const uint8_t stepperPins[stepperPinCnt])
:pins(stepperPins){
  for(int i = 0; i < stepperPinCnt; i++){
    pinMode(stepperPins[i], OUTPUT);
  }
  digitalWrite(stepperPins[4], HIGH);
}

void Stepper::stepAndUpdate(){
  int32_t pos = stepperPos + stepperPosDelta;
  if(pos > stepperPosReal){
    stepperPosReal++;
  }else if(pos < stepperPosReal){
    stepperPosReal--;
  }else{
    for(int i = 0; i < stepperPinCnt; i++){
      //digitalWrite(pins[stepper][i],LOW);
    }
    return;
  }
  int stepperState = stepperPosReal % stepperStateCnt;
  if(stepperState < 0){
    stepperState += stepperStateCnt;
  }
  for(int i = 0; i < stepperStatePinCnt; i++){
    digitalWrite(pins[i],fsmStepper[stepperState][i]);
  }
}
