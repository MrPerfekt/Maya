#include "encoder.h"
#include "stepper.h"
#include "timer.h"
#include "differenceIntensityLaserDetector.h"
#include "interruptLaserDetector.h"
#include "encoder.h"
#include "simpleLowPassFilter.h"
#include "diodePositionController.h"
#include "counter.h"
#include "debug.h"

//#define APPROXIMATION
//#define DIODE_SEARCH
//#define MATHEMATIC

//
//==LEDs==
//
const uint8_t ledCnt = 9;
const uint8_t ledPins[ledCnt] = { 22, 24, 26, 28, 30, 32, 34, 36, 38 };

//
//==LightBarrier==
//
const uint8_t lightBarrierCount = 4;
const uint8_t lightBarrierPins[lightBarrierCount] = { 42,43,44,45 };
volatile bool lightBarrierAction[lightBarrierCount] = { false };
void lightBarrier0() {
	lightBarrierAction[0] = true;
}
void lightBarrier1() {
	lightBarrierAction[1] = true;
}
void lightBarrier2() {
	lightBarrierAction[2] = true;
}
void lightBarrier3() {
	lightBarrierAction[3] = true;
}
void(*lightBarrierFP[lightBarrierCount])() = { lightBarrier0 ,lightBarrier1, lightBarrier2,lightBarrier3 };

/*
  //==Encoder==
  const uint8_t pinEncoder = 11;
  unsigned long lastEncoderTime = 0;
  unsigned long lastEncoderTimeDiff;
  int encoderPos=0;
*/

//
//==KnobEncoder==
//
const uint8_t knobCount = 2;
uint8_t pinKnobEncoderButton[knobCount] = { 52, 53 };
uint8_t pinKnobEncoder[knobCount][2] = { {48, 50}, {49, 51} };

volatile bool buttonPressed[knobCount] = { false };
volatile bool encoderChanged[knobCount] = { false };
void knobEncoderPos0() {
	encoderChanged[0] = true;
}
void knobEncoderButton0() {
	buttonPressed[0] = true;
}
void knobEncoderPos1() {
	encoderChanged[1] = true;
}
void knobEncoderButton1() {
	buttonPressed[1] = true;
}
void(*knobEncoderFP[knobCount])() = { knobEncoderPos0, knobEncoderPos1 };
Encoder knobEncoder[knobCount] = { Encoder(pinKnobEncoder[0], knobEncoderFP[0]), Encoder(pinKnobEncoder[1], knobEncoderFP[1]) };

//
//==Stepper==
//
double stepperFrequency01 = 800;
double stepperFrequency2 = 200;
const uint8_t stepperCount = 3;
const uint8_t stepperPins[stepperCount][4] = { {23, 25, 27, 29}, { 31, 33, 35, 37 }, { 8, 9, 10, 11 } };
Stepper* stepper[stepperCount] = { new Stepper(Stepper::unipolar, stepperPins[0]), new Stepper(Stepper::unipolar, stepperPins[1]), new Stepper(Stepper::bipolar, stepperPins[2]) };
#define STEPPER_H 0
#define STEPPER_V 1

void TC3_Handler() { //3.63us, 0.29%
	INTERRUPT_TIMING_WRITE_A(HIGH);
	TC_GetStatus(TC1, 0);
	stepper[0]->stepAndUpdate();
	stepper[1]->stepAndUpdate();
	INTERRUPT_TIMING_WRITE_A(LOW);
}
void TC4_Handler() {//3.047us, 0.06%
	INTERRUPT_TIMING_WRITE_B(HIGH);
	TC_GetStatus(TC1, 1);
	stepper[2]->stepAndUpdate(); 
	INTERRUPT_TIMING_WRITE_B(LOW);
}

//
// Laser
//
const uint8_t laserPin = 41;
double laserDuty = 0.9;
double laserFrequency = 200;
double laserMeasureTime = 0.000001 * 50;
double laserLighteningTime = laserDuty/laserFrequency - laserMeasureTime;
double laserWaitTime = laserLighteningTime + laserMeasureTime;
const uint8_t photoDiodeCount = 4;
const uint8_t photoDiodePin[photoDiodeCount] = { A0,A1,A2,A3 };
volatile double* photoDiodeIntensity;
uint16_t filterTimeConstant = 1;
DifferenceIntensityLaserDetector* laserDetector = new DifferenceIntensityLaserDetector(photoDiodeCount,laserPin,photoDiodePin);
Filter<double> *filter[photoDiodeCount] = { new SimpleLowPassFilter<double>(filterTimeConstant), new SimpleLowPassFilter<double>(filterTimeConstant), new SimpleLowPassFilter<double>(filterTimeConstant), new SimpleLowPassFilter<double>(filterTimeConstant) };
InterruptLaserDetector* interruptLaserDetector = InterruptLaserDetector::initInstance(laserDetector, filter, laserFrequency, laserWaitTime);

//
// PositionController
//
PositionerCounter* counter = new Counter(lightBarrierPins[0], 0);
double steppsPerRevolution = 100;
bool useTimeCorrection = false;
DiscPositioner* discPositioner = new DiscPositioner(counter, steppsPerRevolution, useTimeCorrection);
PositionController* positionController = new DiodePositionController(stepper[0],stepper[1],discPositioner,interruptLaserDetector);
double positionControllerUpdateFrequency = 20;
/*void TC6_Handler() {
	TC_GetStatus(TC2, 0);
	positionController->update();
}*/



void setup() {
	//==Serial==
	Serial.begin(9600);

	//==Knob Encoder==
	for (int i = 0; i < knobCount; i++) {
		knobEncoder[i].init();
	}
	attachInterrupt(pinKnobEncoderButton[1], knobEncoderButton1, FALLING);
	attachInterrupt(pinKnobEncoderButton[0], knobEncoderButton0, FALLING);

	//==Stepper==
	for (int i = 0; i < stepperCount; i++) {
		stepper[i]->init();
	}
	startTimer(TC1, 0, TC3_IRQn, stepperFrequency01);
	startTimer(TC1, 1, TC4_IRQn, stepperFrequency2);

	//==LEDs==
	for (uint8_t i = 0; i < ledCnt; i++) {
		pinMode(ledPins[i], OUTPUT);
		digitalWrite(ledPins[i], LOW);
	}

	//==LightBarrier==
	const uint8_t lightBarrierCount = 4;
	const uint8_t lightBarrierPins[lightBarrierCount] = { 42,43,44,45 };
	for (int i = 0; i < lightBarrierCount; i++) {
		attachInterrupt(lightBarrierPins[i], lightBarrierFP[i], CHANGE);
	}

	//==Laser==
	interruptLaserDetector->init();
	photoDiodeIntensity = interruptLaserDetector->valuesFiltered;

	//==PositionController==
	//startTimer(TC2, 0, TC6_IRQn, positionControllerUpdateFrequency);


	  //==DebugPin==
	pinMode(debugPin, OUTPUT);
	digitalWrite(debugPin, LOW);
	/*
		  //==Encoder==
		  attachInterrupt(pinEncoder, encoderPinChanged, CHANGE);
		*/
	Serial.println("init finish");
}

/*
  void encoderPinChanged() {
  encoderPos++;
  unsigned long time = millis();
  lastEncoderTimeDiff = time - lastEncoderTime;
  lastEncoderTime = time;
  }
*/


void loop() {
	for (int i = 0; i < knobCount; i++) {
		if (buttonPressed[i]) {
			buttonPressed[i] = false;
			Serial.print("but ");
			Serial.println(i);
			if (i == 0) {
				stepper[2]->posDelta += 200;
			}
			if (i == 1) {
				Serial.print(photoDiodeIntensity[0]);
				Serial.print("\t");
				Serial.print(photoDiodeIntensity[1]);
				Serial.print("\t");
				Serial.print(photoDiodeIntensity[2]);
				Serial.print("\t");
				Serial.println(photoDiodeIntensity[3]);
			}
		}
		if (encoderChanged[i]) {
			encoderChanged[i] = false;
			Serial.print("enc ");
			Serial.print(i);
			Serial.print(" ");
			Serial.println(knobEncoder[0].pos);
			//startTimer(TC1, 0, TC3_IRQn, stepperFrequency+knobEncoder[0].pos);
			stepper[0]->posDelta = knobEncoder[0].pos * 10;
			stepper[1]->posDelta = knobEncoder[1].pos * 10;
		}
	}
	for (int i = 0; i < lightBarrierCount; i++) {
		if (lightBarrierAction[i]) {
			lightBarrierAction[i] = false;
			Serial.print("light ");
			Serial.println(i);
		}
	}
	/*
	delay(500);
	Serial.print(photoDiodeIntensity[0]);
	Serial.print("\t");
	Serial.print(photoDiodeIntensity[1]);
	Serial.print("\t");
	Serial.print(photoDiodeIntensity[2]);
	Serial.print("\t");
	Serial.println(photoDiodeIntensity[3]);
	*/
	positionController->update();
}


/*
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
	/
  #endif
  }
*/
