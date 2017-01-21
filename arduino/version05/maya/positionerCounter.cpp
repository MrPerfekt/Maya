#include "discPositioner.h"

PositionerCounter::PositionerCounter(void(*notifyFunction)())
:notifyFunction(notifyFunction){
	timeRingBuffer[0] = millis();
}

void PositionerCounter::setStepTime() {
	timeRingBufferFirst++;
	if (timeRingBufferFirst >= timeRingBufferSize) {
		timeRingBufferFirst -= timeRingBufferSize;
	}
	timeRingBuffer[timeRingBufferFirst] = millis();
	if (timeRingBufferLast == timeRingBufferFirst) {
		timeRingBufferLast++;
		if (timeRingBufferLast >= timeRingBufferSize) {
			timeRingBufferLast -= timeRingBufferSize;
		}
	}
}

double PositionerCounter::getAverageStepTime() {
	return (timeRingBuffer[timeRingBufferLast] - timeRingBuffer[timeRingBufferFirst]) / timeRingBufferSize;
}

uint32_t PositionerCounter::getLatestStepTime() {
	return timeRingBuffer[timeRingBufferFirst];
}

uint32_t PositionerCounter::getSecondLatestStepTime() {
	return timeRingBuffer[timeRingBufferFirst];
}

void PositionerCounter::notify() {
	if (notifyFunction != 0) {
		notifyFunction();
	}
}
void PositionerCounter::step() {
	setStepTime();
	notify();
}