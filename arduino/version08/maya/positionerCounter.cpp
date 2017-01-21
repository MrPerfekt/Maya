#include "discPositioner.h"

/*
uint16_t PositionerCounter::timeRingBufferIncrease(uint16_t input, uint16_t step) {
	input += step;
	while (input >= timeRingBufferSize) {
		input -= timeRingBufferSize;
	}
	return input;
}
uint16_t PositionerCounter::timeRingBufferIncrease(uint16_t input) {
	return timeRingBufferIncrease(input,1);
}
uint16_t PositionerCounter::timeRingBufferDecrease(uint16_t input, uint16_t step) {
	while (input < step) {
		input += timeRingBufferSize;
	}
	input -= step;
	return input;
}
uint16_t PositionerCounter::timeRingBufferDecrease(uint16_t input) {
	return timeRingBufferDecrease(input,1);
}

void PositionerCounter::setStepTime(uint8_t bufferNr) {
	timeRingBufferFirst[bufferNr] = timeRingBufferIncrease(timeRingBufferFirst[bufferNr]);
	timeRingBuffer[bufferNr][timeRingBufferFirst[bufferNr]] = millis();
	if (timeRingBufferLast == timeRingBufferFirst) {
		timeRingBufferLast[bufferNr] = timeRingBufferIncrease(timeRingBufferLast[bufferNr]);
	}
}

double PositionerCounter::getAverageStepTime(uint8_t bufferNr) {
	return (timeRingBuffer[bufferNr][timeRingBufferLast[bufferNr]] - timeRingBuffer[bufferNr][timeRingBufferFirst[bufferNr]]) / (double)timeRingBufferSize;
}

double PositionerCounter::getAverageWithoutLatestStepTime(uint8_t bufferNr) {
	return (timeRingBuffer[bufferNr][timeRingBufferDecrease(timeRingBufferLast[bufferNr])] - timeRingBuffer[bufferNr][timeRingBufferFirst[bufferNr]]) / (double)(timeRingBufferSize-1);
}

uint32_t PositionerCounter::getNLatestStepTime(uint8_t bufferNr, uint16_t n) {
	return timeRingBuffer[bufferNr][timeRingBufferDecrease(timeRingBufferFirst[bufferNr],n)] - timeRingBuffer[bufferNr][timeRingBufferDecrease(timeRingBufferFirst[bufferNr], n+1)];
}
*/

PositionerCounter::PositionerCounter(void(*notifyFunction)())
	:notifyFunction(notifyFunction), direction(true), pos(0) {
	//for (uint8_t i = 0; i < timeRingBufferCnt; i++) {
	//	timeRingBuffer[i][0] = millis();
	//}
}

void PositionerCounter::notify() {
	if (notifyFunction != 0) {
		notifyFunction();
	}
}

void PositionerCounter::step() {
	//setStepTime(0);
	notify();
}

void PositionerCounter::step(uint8_t bufferNr) {
	//setStepTime(bufferNr);
	//lastCntNr = bufferNr;
	notify();
}