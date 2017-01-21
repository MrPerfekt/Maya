#include "encoder.h"
#include "stepper.h"
#include "timer.h"
#include "differenceIntensityLaserDetector.h"
#include "interruptLaserDetector.h"
#include "encoder.h"
#include "simpleLowPassFilter.h"
#include "counter.h"
#include "fakeCounter.h"
#include "debug.h"
#include "diodePositionController.h"
#include "mathematicPositionController.h"
#include "approximationPositionController.h"
#include "remotePositionController.h"
#include "stepperWatcher.h"

//
// LEDs
//
const uint8_t ledCnt = 9;
const uint8_t ledPins[ledCnt] = { 22, 24, 26, 28, 30, 32, 34, 36, 38 };

//
// LightBarrier
//
const uint8_t lightBarrierEncoderNr0 = 0;
const uint8_t lightBarrierEncoderNr1 = 1;
const uint8_t lightBarrierStepperNrH = 2;
const uint8_t lightBarrierStepperNrV = 3;
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

//
// Encoder
//
#define FAKE_ENCODER 0
uint8_t pinEncoder[2] = { lightBarrierPins[lightBarrierEncoderNr0], lightBarrierPins[lightBarrierEncoderNr1]};
volatile bool encoderChanged = false;
void encoderPos() {
	encoderChanged = true;
}
void(*encoderFP)() = { encoderPos };
#if FAKE_ENCODER != 1
PositionerCounter* encoderPositionCounter = new Encoder(pinEncoder, encoderFP);
#else
PositionerCounter* encoderPositionCounter = new FakeCounter(encoderFP);
double fakeCounterFrequency = 10;
void TC0_Handler() {
	TIMER_INTERRUPT_TIMING_WRITE_F(HIGH);
	TC_GetStatus(TC0, 0);
	encoderPositionCounter->update();
	TIMER_INTERRUPT_TIMING_WRITE_F(LOW);
}
#endif

//
// KnobEncoder
//
const uint8_t knobCount = 2;
uint8_t pinKnobEncoderButton[knobCount] = { 52, 53 };
uint8_t pinKnobEncoder[knobCount][2] = { {48, 50}, {49, 51} };

volatile bool buttonPressed[knobCount] = { false };
volatile bool knobEncoderChanged[knobCount] = { false };
void knobEncoderPos0() {
	knobEncoderChanged[0] = true;
}
void knobEncoderButton0() {
	buttonPressed[0] = true;
}
void knobEncoderPos1() {
	knobEncoderChanged[1] = true;
}
void knobEncoderButton1() {
	buttonPressed[1] = true;
}
void(*knobEncoderFP[knobCount])() = { knobEncoderPos0, knobEncoderPos1 };
PositionerCounter* knobEncoder[knobCount] = { new Encoder(pinKnobEncoder[0], knobEncoderFP[0]), new Encoder(pinKnobEncoder[1], knobEncoderFP[1]) };

//
// Stepper
//
#define DO_INIT_STEPPER_POS 1
#define DISC_STEPPER_RUNNING 1
#define DISC_STEPPER_SOFT_START 1

const uint16_t stepperSoftStartParts = 100;
const uint16_t stepperSoftStartTime = 1000;
const uint16_t stepperSoftStartStepTime = softStartTime / softStartParts;

const uint8_t stepperNrH = 0;
const uint8_t stepperNrV = 1;
const uint8_t stepperNrS = 2;
double stepperFrequency01 = 800;
double stepperFrequency2 = 100;
const uint8_t stepperCount = 3;
const uint8_t stepperPins[stepperCount][4] = { {23, 25, 27, 29}, { 31, 33, 35, 37 }, { 8, 9, 10, 11 } };
Stepper* stepper[stepperCount] = { new Stepper(Stepper::unipolar, stepperPins[stepperNrH]), new Stepper(Stepper::unipolar, stepperPins[stepperNrV]), new Stepper(Stepper::bipolar, stepperPins[stepperNrS]) };
const uint8_t stepperWatcherCount = 3;
const uint8_t stepperWatcherInputPins[stepperWatcherCount][4] = { { 22, 24, 26, 28 },{ 30, 32, 34, 36 } };
StepperWatcher* stepperWatcher[stepperWatcherCount] = { new StepperWatcher(stepper[stepperNrH],stepperWatcherInputPins[stepperNrH]), new StepperWatcher(stepper[stepperNrV],stepperWatcherInputPins[stepperNrV]) };

void TC3_Handler() { //3.63us, 0.29%
	TIMER_INTERRUPT_TIMING_WRITE_A(HIGH);
	TC_GetStatus(TC1, 0);
	stepper[0]->stepAndUpdate();
	stepper[1]->stepAndUpdate();
	TIMER_INTERRUPT_TIMING_WRITE_A(LOW);
}
void TC4_Handler() {//3.047us, 0.06%
	TIMER_INTERRUPT_TIMING_WRITE_B(HIGH);
	TC_GetStatus(TC1, 1);
	stepper[2]->stepAndUpdate(); 
	TIMER_INTERRUPT_TIMING_WRITE_B(LOW);
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
uint16_t filterTimeConstant = 1;
DifferenceIntensityLaserDetector* laserDetector = new DifferenceIntensityLaserDetector(photoDiodeCount,laserPin,photoDiodePin);
Filter<double> *filter[photoDiodeCount] = { new SimpleLowPassFilter<double>(filterTimeConstant), new SimpleLowPassFilter<double>(filterTimeConstant), new SimpleLowPassFilter<double>(filterTimeConstant), new SimpleLowPassFilter<double>(filterTimeConstant) };
InterruptLaserDetector* interruptLaserDetector = InterruptLaserDetector::initInstance(laserDetector, filter, laserFrequency, laserWaitTime);

//
// PositionController
//
double encoderSteppsPerRevolution = 100;
bool encoderUseTimeCorrection = true;
uint8_t encoderBigMarkSize = 3;
DiscPositioner* discPositioner = DiscPositioner::initInstance(encoderPositionCounter, encoderSteppsPerRevolution, encoderBigMarkSize, encoderUseTimeCorrection);
double positionControllerUpdateFrequency = 20;
PositionController* positionController = DiodePositionController::initInstance(stepper[0], stepper[1], discPositioner, interruptLaserDetector);
//PositionController* positionController = MathematicPositionController::initInstance(stepper[0], stepper[1], discPositioner);
//PositionController* positionController = ApproximationPositionController::initInstance(stepper[0], stepper[1], discPositioner);
//PositionController* positionController = RemotePositionController::initInstance(stepperWatcher[0], stepperWatcher[1], discPositioner);


//
// ==== SETUP ====
//
void setup() {
	//==Serial==
	Serial.begin(9600);

	//==Stepper==
	for (int i = 0; i < stepperCount; i++) {
		stepper[i]->init();
	}
	startTimer(TC1, 0, TC3_IRQn, stepperFrequency01);

#if DO_INIT_STEPPER_POS == 1
	stepper[stepperNrH]->pos = stepper[stepperNrV]->pos = Stepper::posInfinity;
	bool finishedH;
	bool finishedV;
	do {
		bool finishedH = stepper[stepperNrH]->initPosUpdate(digitalRead(lightBarrierPins[lightBarrierStepperNrH]));
		bool finishedV = stepper[stepperNrH]->initPosUpdate(digitalRead(lightBarrierPins[lightBarrierStepperNrV]));
	} while (finishedH && finishedV);
#endif

#if DISC_STEPPER_RUNNING == 1
	stepper[stepperNrS]->pos = Stepper::posInfinity;
#if DISC_STEPPER_SOFT_START == 1
	for (uint16_t i = 0; i < stepperSoftStartParts; i++) {
		startTimer(TC1, 1, TC4_IRQn, (stepperFrequency2 * i) / stepperSoftStartParts);
		delay(stepperSoftStartStepTime);
	}
#endif
#endif
	startTimer(TC1, 1, TC4_IRQn, stepperFrequency2);

	//==Knob Encoder==
	for (int i = 0; i < knobCount; i++) {
		knobEncoder[i]->init();
	}
	attachInterrupt(pinKnobEncoderButton[1], knobEncoderButton1, FALLING);
	attachInterrupt(pinKnobEncoderButton[0], knobEncoderButton0, FALLING);

	//==LEDs==
	for (uint8_t i = 0; i < ledCnt; i++) {
		pinMode(ledPins[i], OUTPUT);
		digitalWrite(ledPins[i], LOW);
	}

	//==LightBarrier==
	const uint8_t lightBarrierCount = 4;
	const uint8_t lightBarrierPins[lightBarrierCount] = { 42,43,44,45 };
	for (int i = 0; i < 2; i++) {
		attachInterrupt(lightBarrierPins[i], lightBarrierFP[i], FALLING);
	}
	for (int i = 2; i < lightBarrierCount; i++) {
		attachInterrupt(lightBarrierPins[i], lightBarrierFP[i], CHANGE);
	}

	//==Laser==
	interruptLaserDetector->init();

	 //==DebugPin==
	pinMode(debugPin, OUTPUT);
	digitalWrite(debugPin, LOW);
	
	//==Encoder==
	encoderPositionCounter->init();
	discPositioner->init();
#if FAKE_ENCODER == 1
	startTimer(TC0, 0, TC0_IRQn, fakeCounterFrequency);
#endif

	//==PositionController==
	positionController->init();

	Serial.println("init finish");
}

//
// ==== LOOP ====
//
void loop() {
	if(millis() > 1000)
	for (int i = 0; i < knobCount; i++) {
		if (buttonPressed[i]) {
			buttonPressed[i] = false;
			Serial.print("but ");
			Serial.println(i);
			if (i == 0) {
				stepper[2]->posDelta += 200;
			}
			if (i == 1) {
				Serial.print(stepper[0]->posDelta);
				Serial.print("\t");
				Serial.print(stepper[1]->posDelta);
				Serial.print("\t");
				Serial.print(stepper[0]->pos);
				Serial.print("\t");
				Serial.print(stepper[1]->pos);
				Serial.print("\t");
				volatile double* photoDiodeIntensity = interruptLaserDetector->valuesFiltered;
				Serial.print(photoDiodeIntensity[0]);
				Serial.print("\t");
				Serial.print(photoDiodeIntensity[1]);
				Serial.print("\t");
				Serial.print(photoDiodeIntensity[2]);
				Serial.print("\t");
				Serial.println(photoDiodeIntensity[3]);
			}
		}
		if (knobEncoderChanged[i]) {
			knobEncoderChanged[i] = false;
			stepper[0]->posDelta = knobEncoder[0]->pos * 10;
			stepper[1]->posDelta = knobEncoder[1]->pos * 10;
			Serial.print("enc ");
			Serial.print(i);
			Serial.print("\t");
			Serial.print(knobEncoder[0]->pos);
			Serial.print("\t");
			Serial.println(knobEncoder[1]->pos);
		}
	}
	for (int i = 0; i < lightBarrierCount; i++) {
		if (lightBarrierAction[i]) {
			lightBarrierAction[i] = false;
			Serial.print("light ");
			Serial.println(i);
		}
	}
	positionController->update();
}