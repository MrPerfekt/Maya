#include "stepperWatcher.h"
#include "Arduino.h"

uint8_t StepperWatcher::stepperWatcherCount = 0;

StepperWatcher::StepperWatcher(Stepper *stepper, const uint8_t *inputPins)
	: inputPinState(new uint8_t[maxStepperWatcherCount]), stepper(stepper), inputPins(inputPins), pinCnt(stepper->pinCnt){
}

void StepperWatcherPinChanged() {
	for (uint8_t i = 0; i < StepperWatcher::stepperWatcherCount; i++) {
		StepperWatcher::allStepperWatcher[i]->update();
	}
}

void StepperWatcher::init() {
	if (stepperWatcherCount < maxStepperWatcherCount) {
		allStepperWatcher[stepperWatcherCount] = this;
		stepperWatcherCount++;
		for (uint8_t i = 0; i < 3; i++) {
			attachInterrupt(i, StepperWatcherPinChanged, CHANGE);
		}
	}
	for (int i = 0; i < pinCnt; i++) {
		pinMode(inputPins[i], INPUT);
	}
}

void StepperWatcher::update() {
	for (uint8_t i = 0; i < pinCnt; i++) {
		uint8_t in = digitalRead(inputPins[i]);
		if (inputPinState[i] != in) {
			digitalWrite(stepper->pins[i],in);

			inputPinState[i] = in;
		}
	}
}