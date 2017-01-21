#include "discPositioner.h"

DiscPositioner* DiscPositioner::discPositioner;
DiscPositioner::DiscPositioner(PositionerCounter* positionerCounter, double steppsPerRevolution, uint8_t bigMarkSize, bool useTimeCorrection)
	:positionerCounter(positionerCounter), steppsPerRevolution(steppsPerRevolution), bigMarkSize(max(1, bigMarkSize)), useTimeCorrection(useTimeCorrection) {
}
DiscPositioner* DiscPositioner::getInstance() {
	return discPositioner;
}
DiscPositioner* DiscPositioner::initInstance(PositionerCounter* positionerCounter, double steppsPerRevolution, uint8_t bigMarkSize, bool useTimeCorrection) {
	if (discPositioner == NULL) {
		discPositioner = new DiscPositioner(positionerCounter, steppsPerRevolution, bigMarkSize, useTimeCorrection);
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
	if(positionerCounter->getLatestStepTime() > positionerCounter->getSecondLatestStepTime()*3){//TODO: Replace 3 by constant
		
	}
}
double DiscPositioner::getAngle() {
	double counter = counterOffset + positionerCounter->pos;
	if (useTimeCorrection) {
		counter += min(bigMarkSize,(millis() - positionerCounter->getLatestStepTime()) / positionerCounter->getAverageStepTime());
	}
	while (counter >= steppsPerRevolution) {
		counterOffset -= steppsPerRevolution;
		counter = counterOffset + positionerCounter->pos;
	}
	while (counter < 0) {
		counterOffset += steppsPerRevolution;
		counter = counterOffset + positionerCounter->pos;
	}
	return (counter * 360.0 / steppsPerRevolution);
}
