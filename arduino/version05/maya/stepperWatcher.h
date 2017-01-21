#ifndef STEPPER_WATCHER
#define STEPPER_WATCHER

#include "Arduino.h"
#include "Stepper.h"

class StepperWatcher {
	const uint8_t *inputPins;
	uint8_t *inputPinState;
	uint8_t pinCnt;
public:
	static const uint8_t maxStepperWatcherCount = 2;
	static uint8_t stepperWatcherCount;
	static StepperWatcher *allStepperWatcher[maxStepperWatcherCount];

	static const uint8_t unipolar = 0;
	static const uint8_t bipolar = 1;
	static const int32_t posInfinity = INT32_MAX;
	static const int32_t negInfinity = INT32_MIN;

	volatile int32_t pos = 0;
	volatile int32_t posDelta = 0;
	volatile int32_t posReal = 0;

	Stepper *stepper;

	StepperWatcher(Stepper* stepper, const uint8_t* inputPins);
	void init();
	void update();
};

#endif
