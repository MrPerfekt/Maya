#ifndef DISC_POSITIONER
#define DISC_POSITIONER

#include "Arduino.h"
#include "positionerCounter.h"

class DiscPositioner { // Singelton
	const double steppsPerRevolution;
	double counterOffset = 0;
	const bool useTimeCorrection;
	const uint8_t bigMarkSize;
	const uint8_t encoderSteppDistance;

	static DiscPositioner* discPositioner;
	DiscPositioner(PositionerCounter* positionerCounter, double steppsPerRevolution, uint8_t bigMarkSize, uint8_t encoderSteppDistance, bool useTimeCorrection);
public:
	bool defaultDir = false;
	uint16_t lastDirChange = 0;
	PositionerCounter* positionerCounter;
	static DiscPositioner* getInstance();
	static DiscPositioner* initInstance(PositionerCounter* positionerCounter, double steppsPerRevolution, uint8_t bigMarkSize, uint8_t encoderSteppDistance, bool useTimeCorrection);

	void init();
	void findBigMark();
	double getAngle();
};

#endif
