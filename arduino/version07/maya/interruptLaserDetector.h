#ifndef INTERRUPT_LASER_DETECTOR
#define INTERRUPT_LASER_DETECTOR

#include "Arduino.h"
#include "differenceIntensityLaserDetector.h"
#include "filter.h"

class InterruptLaserDetector { // Singelton
private:
	static constexpr Tc *tc = TC1;
	static const uint32_t channel = 2;
	static const IRQn_Type irq = TC5_IRQn;

	uint8_t state = false;
	DifferenceIntensityLaserDetector* normalDetector;
	const uint32_t frequencyMeasure;
	const uint32_t frequencySleep;
	Filter<double>** filter;
	bool running = true;
	const bool useFilter;

	static InterruptLaserDetector* interruptDetector;
	InterruptLaserDetector(DifferenceIntensityLaserDetector* normalDetector, Filter<double>** filter, double frequency, double waitTime, void(*notifyFunction)(), bool useFilter);
	void notify();
public:
	volatile bool measurementFinished;

	static InterruptLaserDetector* getInstance();
	static InterruptLaserDetector* initInstance(DifferenceIntensityLaserDetector* normalDetector, Filter<double>** filter, double frequency, double waitTime, void(*notifyFunction)(), bool useFilter);

	void(*notifyFunction)();

	void setRunning(bool running);
	void init();
	volatile uint16_t* measuredValues;
	volatile double* valuesFiltered;//0-1023
	void update();
};

#endif