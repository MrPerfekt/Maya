#ifndef INTERRUPT_LASER_DETECTOR
#define INTERRUPT_LASER_DETECTOR

#include "Arduino.h"
#include "differenceIntensityLaserDetector.h"
#include "filter.h"

class InterruptLaserDetector {
private:
	static constexpr Tc *tc = TC1;
	static const uint32_t channel = 2;
	static const IRQn_Type irq = TC5_IRQn;
	static InterruptLaserDetector* interruptDetector;

	uint8_t state = false;
	DifferenceIntensityLaserDetector* normalDetector;
	double frequency;
	double waitTime;
	Filter<double>** filter;
	InterruptLaserDetector(DifferenceIntensityLaserDetector* normalDetector, Filter<double>** filter, double frequency, double waitTime);
public:
	static InterruptLaserDetector* getInstance();
	static InterruptLaserDetector* initInstance(DifferenceIntensityLaserDetector* normalDetector, Filter<double>** filter, double frequency, double waitTime);

	void init();
	volatile double* valuesFiltered;
	void update();
};

#endif