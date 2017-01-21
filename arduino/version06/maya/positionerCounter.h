#ifndef POSITIONER_COUNTER
#define POSITIONER_COUNTER

#include "Arduino.h"

class PositionerCounter {
	/*
	static uint16_t timeRingBufferIncrease(uint16_t input);
	static uint16_t timeRingBufferIncrease(uint16_t input, uint16_t step);
	static uint16_t timeRingBufferDecrease(uint16_t input, uint16_t step);
	static uint16_t timeRingBufferDecrease(uint16_t input);
	*/
protected:
	void notify();
	void setStepTime(uint8_t bufferNr);
	void step();
	void step(uint8_t bufferNr);
public:
	uint8_t lastCntNr = 0;
	void(*notifyFunction)();
	PositionerCounter(void(*notifyFunction)());
	//static const uint16_t timeRingBufferSize = 50;
	//static const uint16_t timeRingBufferCnt = 2;
	//uint16_t timeRingBufferLast[timeRingBufferCnt] = { 0 };
	//uint16_t timeRingBufferFirst[timeRingBufferCnt] = { 0 };
	//uint32_t timeRingBuffer[timeRingBufferCnt][timeRingBufferSize] = { {0}, {0} };
	int32_t pos = 0;
	volatile bool direction;
	virtual void init() = 0;
	virtual void update() = 0;
	//virtual double getAverageStepTime(uint8_t buffernr);
	////virtual double getTotalAverageStepTime();
	//virtual double getAverageWithoutLatestStepTime(uint8_t buffernr);
	//virtual uint32_t getNLatestStepTime(uint8_t buffernr, uint16_t n);
	////virtual uint32_t getTotalNLatestStepTime(uint16_t n);
};

#endif
