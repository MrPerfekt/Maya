#ifndef DISC_POSITIONER
#define DISC_POSITIONER

#include "Arduino.h"
#include "positionerCounter.h"

class DiscPositioner { // Singelton
	PositionerCounter* positionerCounter;
	const double steppsPerRevolution;
	double counterOffset = 0;
	const bool useTimeCorrection;
	const uint8_t bigMarkSize;

	static DiscPositioner* discPositioner;
	DiscPositioner(PositionerCounter* positionerCounter, double steppsPerRevolution, uint8_t bigMarkSize, bool useTimeCorrection);
public:
	static DiscPositioner* getInstance();
	static DiscPositioner* initInstance(PositionerCounter* positionerCounter, double steppsPerRevolution, uint8_t bigMarkSize, bool useTimeCorrection);

	void init();
	void findBigMark();
	double getAngle();
};

#endif
