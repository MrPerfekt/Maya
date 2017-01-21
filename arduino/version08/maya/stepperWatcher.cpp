#include "stepperWatcher.h"
#include "Arduino.h"

const uint8_t StepperWatcher::fsmCoilPin[4][pinCnt] = { //stop, forward, reverse, invalid // (HIGH, LOW)^2 // coilState = pin[0]*2 + pin[1]
	{ 0,0 },
	{ 0,1 },
	{ 1,0 },
	{ 1,1 }
};
const uint8_t StepperWatcher::fsmStateCoil[stateCnt][coilCnt] = {
	{ 2,0 },
	{ 2,2 },
	{ 0,2 },
	{ 1,2 },
	{ 1,0 },
	{ 1,1 },
	{ 0,1 },
	{ 2,1 }
};

uint8_t StepperWatcher::stepperWatcherCount = 0;
StepperWatcher *StepperWatcher::allStepperWatcher[maxStepperWatcherCount];

StepperWatcher::StepperWatcher(Stepper *stepper, const uint8_t inputPins[pinCnt], uint32_t minStepTime)
	: inputPinState(new uint8_t[maxStepperWatcherCount]), stepper(stepper), inputPins(inputPins), minStepTime(minStepTime){
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
			digitalWrite(stepper->pins[i], in);

			if (micros() - lastStepTime < minStepTime) {
				errorType = errorTypeToFastMotorSpeed;
				continue;
			}
			lastStepTime = micros();

			inputPinState[i] = in;
			uint8_t coilState0 = inputPinState[0] * 2 + inputPinState[1];
			uint8_t coilState1 = inputPinState[2] * 2 + inputPinState[3];
			if (coilState0 == 3 || coilState1 == 3) {
				errorType = errorTypeBothCoilsActuated;
				continue;
			}
			uint8_t state = inputStateOff;
			for (state = 0; state < stateCnt; state++) {
				if (fsmStateCoil[state][0] == coilState0 && fsmStateCoil[state][1] == coilState1) {
					break;
				}
			}
			if (inputState == inputStateOff) {
				inputState = state;
				continue;
			}
			if (state == inputStateOff) {
				errorType = errorTypeMotorTurnedOff;
				continue;
			}
			if (abs((int16_t)inputState - (int16_t)state) > 1) {
				errorType = errorTypeWrongStateCombination;
				continue;
			}
		}
	}
}