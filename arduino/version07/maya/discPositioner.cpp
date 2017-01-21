#include "discPositioner.h"

DiscPositioner* DiscPositioner::discPositioner;
DiscPositioner::DiscPositioner(PositionerCounter* positionerCounter, double steppsPerRevolution, uint8_t bigMarkSize, uint8_t encoderSteppDistance, bool useTimeCorrection)
	:positionerCounter(positionerCounter), steppsPerRevolution(steppsPerRevolution), bigMarkSize(max(1, bigMarkSize)), useTimeCorrection(useTimeCorrection), encoderSteppDistance(encoderSteppDistance){
}
DiscPositioner* DiscPositioner::getInstance() {
	return discPositioner;
}
DiscPositioner* DiscPositioner::initInstance(PositionerCounter* positionerCounter, double steppsPerRevolution, uint8_t bigMarkSize, uint8_t encoderSteppDistance, bool useTimeCorrection) {
	if (discPositioner == NULL) {
		discPositioner = new DiscPositioner(positionerCounter, steppsPerRevolution, bigMarkSize, encoderSteppDistance, useTimeCorrection);
	}
	return getInstance();
}

void discPositionerFindBigMark() {
	DiscPositioner* instance = DiscPositioner::getInstance();
	if (instance != NULL) {
		instance->findBigMark();
	}
}
void DiscPositioner::init() {
	positionerCounter->notifyFunction = discPositionerFindBigMark;
}
void DiscPositioner::findBigMark() {
	if (positionerCounter->direction != defaultDir) {
		if (lastDirChange == 1) {
			defaultDir = positionerCounter->direction;
		} else if (lastDirChange == 11) {
			positionerCounter->pos += 4;
		} else {
			positionerCounter->pos = 0;
			lastDirChange = 0;
		}
	}
	lastDirChange++;
	
	/*
	if(bigMarkSize != 1 && positionerCounter->getNLatestStepTime(0,0) > positionerCounter->getAverageStepTime(0)*2.5){//TODO: Replace 3 by constant
		counterOffset -= positionerCounter->pos;
		Serial.println("a");
	}
	if (positionerCounter->getNLatestStepTime(1, 0) > positionerCounter->getAverageStepTime(1)*2.5 && counterOffset == encoderSteppDistance) {
		counterOffset += 4;
		Serial.println("b");
	}
	*/
}
double DiscPositioner::getAngle() {
	//if (useTimeCorrection) {
	//	uint8_t cntNr = positionerCounter->lastCntNr;
	//	counter += min(bigMarkSize,(millis() - positionerCounter->getNLatestStepTime(cntNr,0)) * 2 / positionerCounter->getAverageStepTime(cntNr));
	//}
	while (positionerCounter->pos >= steppsPerRevolution) {
		positionerCounter->pos -= steppsPerRevolution;
	}
	while (positionerCounter->pos < 0) {
		positionerCounter->pos += steppsPerRevolution;
	}
	return (positionerCounter->pos * 360.0 / steppsPerRevolution);
}