#ifndef DISC_POSITIONER
#define DISC_POSITIONER

#include "Arduino.h"
#include "positionerCounter.h"

class DiscPositioner {
	PositionerCounter* positionerCounter;
	const double steppsPerRevolution;
	double counterOffset = 0;
	const bool useTimeCorrection;
public:
	DiscPositioner(PositionerCounter* positionerCounter, double steppsPerRevolution, bool useTimeCorrection);
	void init();
	void update();
	double getAngle();
};

#endif
