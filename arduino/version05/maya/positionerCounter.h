#ifndef POSITIONER_COUNTER
#define POSITIONER_COUNTER

#include "Arduino.h"

class PositionerCounter {
protected:
	void notify();
	void setStepTime();
	void step();
public:
	void(*notifyFunction)();
	PositionerCounter(void(*notifyFunction)());
	static const uint8_t timeRingBufferSize = 4;
	uint8_t timeRingBufferLast = 0;
	uint8_t timeRingBufferFirst = 0;
	uint32_t timeRingBuffer[timeRingBufferSize] = { 0 };
	int32_t pos = 0;
	virtual void init() = 0;
	virtual void update() = 0;
	double getAverageStepTime();
	uint32_t getLatestStepTime();
	uint32_t getSecondLatestStepTime();
};

#endif
