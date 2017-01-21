//uint8_t dummy;
#include "knobEncoder.h"
#include "stepper.h"
#include "timer.h"
#include "differenceIntensityLaserDetector.h"

//#define APPROXIMATION
//#define DIODE_SEARCH
//#define MATHEMATIC

//==LEDs==
const uint8_t ledCnt = 9;
const uint8_t ledPins[ledCnt]={36,38,40,42,44,46,48,50,52};

//==Encoder==
const uint8_t pinEncoder = 11;
unsigned long lastEncoderTime = 0;
unsigned long lastEncoderTimeDiff;
int encoderPos=0;

//==KnobEncoder==
const uint8_t knobCount = 2;
uint8_t pinsKnobEncoder[knobCount][3]={{26,22,24},{27,23,25}};

//==Stepper==
const uint8_t stepperCount = 2;
const uint8_t stepperPins[stepperCount][5] = {{28,30,32,34,2},{29,31,33,35,3}};
Stepper stepper[stepperCount] = {Stepper(stepperPins[0]), Stepper(stepperPins[1])};
#define STEPPER_H 0
#define STEPPER_V 1

//==Laser==
const uint8_t pinLaser = 11;
const uint8_t photoDiodeCnt = 4;
const uint8_t photoDiodePin[photoDiodeCnt] = {A0,A1,A2,A3};
DifferenceIntensityLaserDetector laserDetector = DifferenceIntensityLaserDetector(photoDiodeCnt, pinLaser, photoDiodePin);
// 1 4
// 2 3
#define PD_LEFT_UP 0
#define PD_LEFT_DOWN 1
#define PD_RIGHT_DOWN 2
#define PD_RIGHT_UP 3

//==DebugPin==
const uint8_t debugPin = 53;
volatile uint8_t debugPinValue = false;
  
void setup() {
  //==DebugPin==
  pinMode(debugPin,OUTPUT);
  digitalWrite(debugPin,LOW);
  
  //==LEDs==
  for(uint8_t i = 0; i < ledCnt;i++){
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  
  //==Serial==
  Serial.begin(9600);
  
  //==Knob Encoder==
  void  (*fp[][2])() = {{0,0},{0,0}};
  setKnobEncoders(knobCount, pinsKnobEncoder,fp);
  uint8_t test = knobEncoderPos[0];

  //==Encoder==
  attachInterrupt(pinEncoder, encoderPinChanged, CHANGE);
  
  //==Stepper==
  double frequency = 800;
  startTimer(TC1, 0, TC3_IRQn, frequency);
}
ISR(TIMER1_COMPA_vect)
{
  for(uint8_t i = 0; i < stepperCount; i++){
    stepper[i].stepAndUpdate();
  }
}
void encoderPinChanged(){
  encoderPos++;
  unsigned long time = millis();
  lastEncoderTimeDiff = time - lastEncoderTime;
  lastEncoderTime = time;
}

// -120, -100, -80, -60, -40, -20, 0, 20, 40, 60, 80, 100
#define VALUES_V 0
#define VALUES_H 1
const int32_t encoderPosValues[12][2] = {{-20,68},{-15,57},{-10,46},{-7,34},{-4,22},{-2,11},{-3,0},{-4,-13},{-8,-26},{-12,-38},{-17,-48},{-23,-60}};
uint32_t loopCnt = 0;
void loop(){
  loopCnt++;
  //digitalWrite(debugPin,loopCnt%2==0);

#ifdef DIODE_SEARCH
    int16_t diodeLU = max(0,(int16_t)photoDiodeValueAfter[PD_LEFT_UP]-(int16_t)photoDiodeValueBefore[PD_LEFT_UP]);
    int16_t diodeLD = max(0,(int16_t)photoDiodeValueAfter[PD_LEFT_DOWN]-(int16_t)photoDiodeValueBefore[PD_LEFT_DOWN]);
    int16_t diodeRU = max(0,(int16_t)photoDiodeValueAfter[PD_RIGHT_UP]-(int16_t)photoDiodeValueBefore[PD_RIGHT_UP]);
    int16_t diodeRD = max(0,(int16_t)photoDiodeValueAfter[PD_RIGHT_DOWN]-(int16_t)photoDiodeValueBefore[PD_RIGHT_DOWN]);
    int16_t horizontal = diodeLU+diodeLD-diodeRU-diodeRD;
    int16_t vertical = diodeLU+diodeRU-diodeLD-diodeRD;
    
    static double lowHor = 0;
    const uint8_t lowPassGrade = 10;
    const uint8_t triggerLevel = 50;
    static uint8_t verDir = 0;
    lowHor = (lowHor*lowPassGrade+horizontal)/(lowPassGrade+1);
    static double lowVer = 0;
    lowVer = (lowVer*lowPassGrade+vertical)/(lowPassGrade+1);

    static double stepperPosV = 0;
    static double stepperPosH = 0;
    if(loopCnt % 6 == 0){
      double encoderAngle = encoderPos*PI/4/180;
      double sinV = sin(encoderAngle);
      double cosV = cos(encoderAngle);
      if(lowVer > triggerLevel){
        stepperPosV += cosV;
        stepperPosH -= sinV;
      }else if(lowVer < -triggerLevel){
        stepperPosV -= cosV;
        stepperPosH += sinV;
      }
      if(lowHor > triggerLevel){
        stepperPosH += cosV;
        stepperPosV += sinV;
      }else if(lowHor < -triggerLevel){
        stepperPosH -= cosV;
        stepperPosV -= sinV;
      }
      stepperPos[STEPPER_V] = (int)stepperPosV;
      stepperPos[STEPPER_H] = (int)stepperPosH;
    }
#endif
#ifdef APPROXIMATION
  int32_t pos = min(100,max(-119,encoderPos));
  uint8_t index0 = (pos+120)/20;
  uint8_t index1 = index0+1;
  int32_t deltaPos = (pos+120) % 20;
  
  int32_t stepperBaseH = encoderPosValues[index0][VALUES_H];
  int32_t stepperBaseV = encoderPosValues[index0][VALUES_V];
  
  int32_t stepperRiseH = encoderPosValues[index1][VALUES_H] - stepperBaseH;
  int32_t stepperRiseV = encoderPosValues[index1][VALUES_V] - stepperBaseV;

  stepperPos[STEPPER_H] = stepperBaseH + (stepperRiseH * deltaPos) / 20;
  stepperPos[STEPPER_V] = stepperBaseV + (stepperRiseV * deltaPos) / 20;
  if(loopCnt % 10000 == 0){
    Serial.print(encoderPos);
    Serial.print(" ");
    Serial.print(deltaPos);
    Serial.print(" ");
    Serial.print(stepperBaseH + (stepperRiseH * deltaPos) / 20);
    Serial.print(" ");
    Serial.println(stepperBaseV + (stepperRiseV * deltaPos) / 20);
  }
#endif

#ifdef MATHEMATIC
  double pendulumDistX = 0;
  double pendulumDistY = 0;
  double pendulumDistZ = 263;
  double pendulumLength = 130;
  double encoderAngle = encoderPos*PI/4/180 + encoderDir*15.0/(double)lastEncoderTimeDiff;
  double posX = pendulumLength*sin(encoderAngle)+pendulumDistX;
  double posY = pendulumLength*(cos(encoderAngle)-1)+pendulumDistY;
  /*
  stepperPos[STEPPER_H] = (int)(4096*asin(posX/(pendulumDistZ+20))/(2*PI)/2);
  stepperPos[STEPPER_V] = (int)(4096*asin(posY/pendulumDistZ)/(2*PI)/2);
  */
#endif
}
