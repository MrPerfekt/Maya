#include "knobEncoder.h"

uint8_t *knobEncoderPos;
const int8_t knobEncoderFsm[4][4]={{0,1,-1,0},{-1,0,0,1},{1,0,0,-1},{0,-1,1,0}};
uint8_t (*knobEncoderPins)[3];
uint8_t (*knobEncoderPinStatus)[3];
uint8_t knobEncoderCount;
void  (*(*knobEncoderNotifyFunctions)[2])();

void knobEncoderPinChanged(){
  for(uint8_t i; i < knobEncoderCount; i++){
    for(uint8_t j; j < 3; j++){
      uint8_t current = digitalRead(knobEncoderPins[i][j]) == HIGH;
      if (knobEncoderPinStatus[i][j] != current){
        if(j==0){
          if(knobEncoderNotifyFunctions[i][0] != 0){
            knobEncoderNotifyFunctions[i][0]();
          }
        }else{
          uint8_t lasta = knobEncoderPinStatus[i][1];
          uint8_t lastb = knobEncoderPinStatus[i][2];
          uint8_t ina = lasta + (j == 1 ? 1 : 0);
          uint8_t inb = lasta + (j == 2 ? 1 : 0);
          knobEncoderPos[i]+=knobEncoderFsm[(ina<<1)+inb][(lasta<<1)+lastb];
          if(knobEncoderNotifyFunctions[i][1] != 0){
            knobEncoderNotifyFunctions[i][1]();
          }
        }
        knobEncoderPinStatus[i][j] = current;
      }
    }
  }
}

void setKnobEncoders(uint8_t count, uint8_t pins[][3], void (*notifyFunctions[][2])()){// Pins: Button, A, B ; Functions: Button changed
//  knobEncoderNotifyFunctions = notifyFunctions;
  knobEncoderCount = count;
  knobEncoderPins = pins;
  knobEncoderPinStatus = new uint8_t[count][3];
  knobEncoderPos = new uint8_t[count];
  knobEncoderNotifyFunctions=notifyFunctions;
  for(uint8_t i; i < knobEncoderCount; i++){
    for(uint8_t j; j < 3; j++){
      attachInterrupt(knobEncoderPins[i][j], knobEncoderPinChanged, CHANGE);
    }
  }
}

