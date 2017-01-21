#include "discPositioner.h"

DiscPositioner::DiscPositioner(PositionerCounter* positionerCounter, double steppsPerRevolution, bool useTimeCorrection)
:positionerCounter(positionerCounter), steppsPerRevolution(steppsPerRevolution), useTimeCorrection(useTimeCorrection){
}
void DiscPositioner::init() {
	positionerCounter->init();
}
void DiscPositioner::update() {
	positionerCounter->update();
}
double DiscPositioner::getAngle() {
	double counter = counterOffset + positionerCounter->pos;
	if (useTimeCorrection) {
		counter += max(1,(millis() - positionerCounter->getLastStepTime()) / positionerCounter->getAverageStepTime());
	}
	while (counter > steppsPerRevolution) {
		counterOffset -= steppsPerRevolution;
		counter = counterOffset + positionerCounter->pos;
	}
	while (counter < 0) {
		counterOffset += steppsPerRevolution;
		counter = counterOffset + positionerCounter->pos;
	}
	return (counter * 360.0 / steppsPerRevolution);
}