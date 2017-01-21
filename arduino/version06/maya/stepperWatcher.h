#ifndef STEPPER_WATCHER
#define STEPPER_WATCHER

#include "Arduino.h"
#include "Stepper.h"

class StepperWatcher {
	static const uint8_t inputStateOff = UINT8_MAX;
	uint8_t inputState = inputStateOff;
	uint32_t lastStepTime = 0;

	const uint32_t minStepTime; //In Microseconds
	const uint8_t *inputPins;
	uint8_t *inputPinState;
public:
	static const uint8_t errorTypeNon = 0;
	static const uint8_t errorTypeBothCoilsActuated = 1;
	static const uint8_t errorTypeWrongStateCombination = 2;
	static const uint8_t errorTypeMotorTurnedOff = 3;
	static const uint8_t errorTypeToFastMotorSpeed = 4;

	static const uint8_t maxStepperWatcherCount = 2;
	static uint8_t stepperWatcherCount;
	static StepperWatcher *allStepperWatcher[maxStepperWatcherCount];

	static const uint8_t stateCnt = 8;
	static const uint8_t coilCnt = 2; //A,B
	static const uint8_t pinCnt = coilCnt*2; //A+,A-,B+,B-
	static const uint8_t fsmCoilPin[4][pinCnt]; //stop, forward, reverse, invalid // (HIGH, LOW)^2 // coilState = pin[0]*2 + pin[1]
	static const uint8_t fsmStateCoil[stateCnt][coilCnt];

	uint8_t errorType = errorTypeNon;

	Stepper *stepper;

	StepperWatcher(Stepper* stepper, const uint8_t inputPins[pinCnt], uint32_t minStepTime);
	void init();
	void update();
};

#endif
