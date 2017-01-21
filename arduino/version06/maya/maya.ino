#include "config.h"
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
// LightBarrier
//
void lightBarrierStepperUpdate();
volatile bool lightBarrierAction[lightBarrierCount] = { false };
void lightBarrier0() {
	lightBarrierAction[0] = true;
}
void lightBarrier1() {
	lightBarrierAction[1] = true;
}
void lightBarrier2() {
	lightBarrierAction[2] = true;
	lightBarrierStepperUpdate();
}
void lightBarrier3() {
	lightBarrierAction[3] = true;
	lightBarrierStepperUpdate();
}
void(*lightBarrierFP[lightBarrierCount])() = { lightBarrier0 ,lightBarrier1, lightBarrier2,lightBarrier3 };

//
// Encoder
//
#if FAKE_ENCODER != 1
PositionerCounter* encoderPositionCounter = new Encoder(pinEncoder, { 0 });
#else
PositionerCounter* encoderPositionCounter = new FakeCounter(encoderFP);
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
Stepper* stepper[stepperCount] = { new Stepper(Stepper::unipolar, stepperPins[stepperNrH]), new Stepper(Stepper::unipolar, stepperPins[stepperNrV]), new Stepper(Stepper::bipolar, stepperPins[stepperNrS]) };
StepperWatcher* stepperWatcher[stepperWatcherCount] = { new StepperWatcher(stepper[stepperNrH],stepperWatcherInputPins[stepperNrH],stepperWatcherMinStepTime), new StepperWatcher(stepper[stepperNrV],stepperWatcherInputPins[stepperNrV],stepperWatcherMinStepTime) };

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
DifferenceIntensityLaserDetector* laserDetector = new DifferenceIntensityLaserDetector(photoDiodeCount, laserPin, photoDiodePin);
Filter<double> *filter[photoDiodeCount] = { new SimpleLowPassFilter<double>(photoDiodeFilterTimeConstant), new SimpleLowPassFilter<double>(photoDiodeFilterTimeConstant), new SimpleLowPassFilter<double>(photoDiodeFilterTimeConstant), new SimpleLowPassFilter<double>(photoDiodeFilterTimeConstant) };
InterruptLaserDetector* interruptLaserDetector = InterruptLaserDetector::initInstance(laserDetector, filter, laserFrequency, laserWaitTime, 0, photoDiodeFilterTimeConstant == 1 ? false : true);

//
// PositionController
//
DiscPositioner* discPositioner = DiscPositioner::initInstance(encoderPositionCounter, encoderSteppsPerRevolution, encoderBigMarkSize, encoderSteppDistance, encoderUseTimeCorrection);
PositionController* positionController;

//
// ==== SETUP ====
//
void setup() {
	//==Serial==
	Serial.begin(460800);

	//==Stepper==
	setupStepper();

	//==Knob Encoder==
	for (int i = 0; i < knobCount; i++) {
		knobEncoder[i]->init();
	}
	attachInterrupt(pinKnobEncoderButton[1], knobEncoderButton1, FALLING);
	attachInterrupt(pinKnobEncoderButton[0], knobEncoderButton0, FALLING);

	//==LightBarrier==
	for (int i = 0; i < lightBarrierCount; i++) {
		pinMode(lightBarrierPins[i], INPUT);
		//attachInterrupt(lightBarrierPins[i], lightBarrierFP[i], CHANGE);
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
	startTimer(TC0, 0, TC0_IRQn, fakeEncoderCounterFrequency);
#endif

	//==PositionController==
	if (positionControllerType == POSITION_CONTROLLER_DIODE) {
		positionController = DiodePositionController::initInstance(stepper[stepperNrH], stepper[stepperNrV], discPositioner, interruptLaserDetector);
	} else if (positionControllerType == POSITION_CONTROLLER_MATHEMATIC) {
		positionController = MathematicPositionController::initInstance(stepper[stepperNrH], stepper[stepperNrV], discPositioner);
	} else if (positionControllerType == POSITION_CONTROLLER_APPROXIMATION) {
		positionController = ApproximationPositionController::initInstance(stepper[stepperNrH], stepper[stepperNrV], discPositioner);
	} else if (positionControllerType == POSITION_CONTROLLER_REMOTE) {
		positionController = RemotePositionController::initInstance(stepperWatcher[stepperNrH], stepperWatcher[stepperNrV], discPositioner, interruptLaserDetector, lightBarrierCount, lightBarrierPins, lightBarrierOutputPins);
	}
	positionController->init();

	Serial.println("init finish");
}
void setupStepper() {
	for (int i = 0; i < stepperCount; i++) {
		stepper[i]->init();
	}
	startTimer(TC1, 0, TC3_IRQn, (uint32_t)stepperFrequency01);

	stepperWatcher[stepperNrH]->init();
	stepperWatcher[stepperNrV]->init();
}

bool lateInitDone = false;
void lateInit() {
	lateInitDone = true;

#if DO_INIT_STEPPER_POS == 1
	bool finishedH;
	bool finishedV;
	do {
		int inH = digitalRead(lightBarrierPins[lightBarrierStepperNrH]);
		int inV = digitalRead(lightBarrierPins[lightBarrierStepperNrV]);
		finishedH = stepper[stepperNrH]->initPosUpdate(inH);
		finishedV = stepper[stepperNrV]->initPosUpdate(inV);
	} while (!finishedH || !finishedV);
	stepper[stepperNrH]->posDelta = stepperStartPosH;
	stepper[stepperNrV]->posDelta = stepperStartPosV;
#endif

#if DISC_STEPPER_RUNNING == 1
	stepper[stepperNrS]->pos = Stepper::posInfinity;
#if DISC_STEPPER_SOFT_START == 1
	for (uint16_t i = 1; i < stepperSoftStartParts; i++) {
		startTimer(TC1, 1, TC4_IRQn, (uint32_t)((stepperFrequency2 * i) / stepperSoftStartParts));
		delay(stepperSoftStartStepTime);
	}
#endif
#endif
	startTimer(TC1, 1, TC4_IRQn, (uint32_t)stepperFrequency2);
}

//
// ==== LOOP ====
//
void loop() {
	if (!lateInitDone) {
		lateInit();
	}
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
				volatile uint16_t* photoDiodeIntensity = interruptLaserDetector->measuredValues;
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
			stepper[stepperNrH]->posDelta = stepperStartPosH + knobEncoder[0]->pos * 10;
			stepper[stepperNrV]->posDelta = stepperStartPosV + knobEncoder[1]->pos * 10;
			Serial.print("enc ");
			Serial.print(i);
			Serial.print("\t");
			Serial.print(knobEncoder[0]->pos);
			Serial.print("\t");
			Serial.println(knobEncoder[1]->pos);

			Serial.print("enc ");
			Serial.print(stepper[stepperNrH]->posDelta);
			Serial.print("\t");
			Serial.println(stepper[stepperNrH]->pos);
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
	//Serial.println(discPositioner->getAngle());
	//delay(500);
	//if (interruptLaserDetector->measuredValues[2] > 400) {
	//	Serial.println(discPositioner->getAngle());
	//	delay(500);
	//}
}