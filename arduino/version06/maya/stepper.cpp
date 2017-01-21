#include "stepper.h"
#include "Arduino.h"

const uint8_t Stepper::fsm[stateCnt][pinCnt] = {
	{ 1,0,0,0 },
	{ 1,0,1,0 },
	{ 0,0,1,0 },
	{ 0,1,1,0 },
	{ 0,1,0,0 },
	{ 0,1,0,1 },
	{ 0,0,0,1 },
	{ 1,0,0,1 }
};

Stepper::Stepper(uint8_t type, const uint8_t pins[pinCnt])
	: type(type), pins(pins) {}

void Stepper::init() {
	for (int i = 0; i < pinCnt; i++) {
		pinMode(pins[i], OUTPUT);
	}
}

void Stepper::stepAndUpdate() {
	int32_t curPos = pos + posDelta;
	if (curPos > posReal) {
		posReal++;
	} else if (curPos < posReal) {
		posReal--;
	} else {
		//for(int i = 0; i < pinCnt; i++){
		//  digitalWrite(pins[stepper][i],LOW);
		//}
		return;
	}
	int state = posReal % stateCnt;
	if (state < 0) {
		state += stateCnt;
	}
	for (int i = 0; i < pinCnt; i++) {
		digitalWrite(pins[i], fsm[state][i]);
	}
}

bool Stepper::initPosUpdate(bool input) {
	switch (initPosStatus) {
	case 0: // Turn bevore start
		if (initPosToMainTurnRisingEdge == input) {
			pos = !initPosMainTurnPositive ? posInfinity : negInfinity;
		}
		initPosStatus++;
		//nobreak;
	case 1: // Turn bevore finished / Start Main Turn
		if (initPosToMainTurnRisingEdge != input) {
			pos = initPosMainTurnPositive ? posInfinity : negInfinity;
			initPosStatus++;
			//nobreak
		} else {
			break;
		}
	case 2: // Main Turn finished / start Micro Turn
		if (initPosToMainTurnRisingEdge == input) {
			pos = posDelta = posReal = 0;
			initPosSlowStepTime = millis();
			initPosStatus++;
			//nobreak
		} else {
			break;
		}
	case 3: // Micro Turn
		if (initPosToMainTurnRisingEdge != input) {
			pos = posDelta = posReal = 0;
			initPosStatus++;
		} else {
			if (millis() - initPosSlowStepTime > initPosSlowStepTimeDif) {
				pos += !initPosMainTurnPositive ? posInfinity : negInfinity;
				initPosSlowStepTime = millis();
			}
		}
		break;
	case 4: // Finished
		return true;
	default:
		initPosStatus = 0;
		break;
	}
	return false;
}