#ifndef DIFFERENCE_INTENSITY_LASER_DETECTOR
#define DIFFERENCE_INTENSITY_LASER_DETECTOR

#include "Arduino.h"

class DifferenceIntensityLaserDetector {
	const uint8_t *pinPhotoDiode;
	volatile uint16_t *photoDiodeValueBefore;
	volatile uint16_t *photoDiodeValueAfter;
public:
	const uint8_t pinLaser;
	const uint8_t photoDiodeCount;
	DifferenceIntensityLaserDetector(uint8_t photoDiodeCount, uint8_t pinLaser, const uint8_t pinPhotoDiode[]);
	void init();
	void MeasureValue();
	void MeasureValueStart();
	volatile uint16_t* MeasureValueFinish();
};

#endif
