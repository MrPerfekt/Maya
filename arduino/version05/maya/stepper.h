#ifndef STEPPER
#define STEPPER

#include "Arduino.h"

class Stepper {
private:
	static const bool initPosToMainTurnRisingEdge = true;
	static const bool initPosMainTurnPositive = true;
	static const uint32_t initPosSlowStepTimeDif = 100;
	uint8_t initPosStatus = 0;
	uint32_t initPosSlowStepTime = 0;
public:
	static const uint8_t typeCnt = 2; //unipolar, bipolar
	static const uint8_t pinCnt = 4;//A+,A-,B+,B-
	static const uint8_t stateCnt = 8;
	const uint8_t fsm[typeCnt][stateCnt][pinCnt] = { {
		{ 1,0,0,0 },
		{ 1,1,0,0 },
		{ 0,1,0,0 },
		{ 0,1,1,0 },
		{ 0,0,1,0 },
		{ 0,0,1,1 },
		{ 0,0,0,1 },
		{ 1,0,0,1 }
		},{
		{ 1,0,0,0 },
		{ 1,0,1,0 },
		{ 0,0,1,0 },
		{ 0,1,1,0 },
		{ 0,1,0,0 },
		{ 0,1,0,1 },
		{ 0,0,0,1 },
		{ 1,0,0,1 }
	} };
	const uint8_t *pins;
	const uint8_t type;

	static const uint8_t unipolar = 0;
	static const uint8_t bipolar = 1;
	static const int32_t posInfinity = INT32_MAX;
	static const int32_t negInfinity = INT32_MIN;

	volatile int32_t pos = 0;
	volatile int32_t posDelta = 0;
	volatile int32_t posReal = 0;

	Stepper(uint8_t type, const uint8_t pins[pinCnt]);
	void init(); //Arduino overrides interupts and pins bevore main routine!!
	void stepAndUpdate();
	bool initPosUpdate(bool input);
};

#endif
