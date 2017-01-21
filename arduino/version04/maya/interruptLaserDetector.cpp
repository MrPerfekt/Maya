#include "interruptLaserDetector.h"
#include "timer.h"
#include "debug.h"

uint16_t* TESTmeasured = { 0 };
InterruptLaserDetector* InterruptLaserDetector::interruptDetector;

void TC5_Handler() {
	INTERRUPT_TIMING_WRITE_C(HIGH);
	InterruptLaserDetector::getInstance()->update();
	INTERRUPT_TIMING_WRITE_C(LOW);
}

InterruptLaserDetector::InterruptLaserDetector(DifferenceIntensityLaserDetector* normalDetector, Filter<double>** filter, double frequency, double waitTime)
:valuesFiltered(new double[normalDetector->photoDiodeCount]), normalDetector(normalDetector), filter(filter), frequency(frequency), waitTime(waitTime) {
	valuesFiltered[0] = 0;
	valuesFiltered[1] = 0;
	valuesFiltered[2] = 0;
	valuesFiltered[3] = 0;
}
InterruptLaserDetector* InterruptLaserDetector::initInstance(DifferenceIntensityLaserDetector* normalDetector, Filter<double>** filter, double frequency, double waitTime) {
	if (interruptDetector == NULL) {
		interruptDetector = new InterruptLaserDetector(normalDetector, filter, frequency, waitTime);
	}
	return getInstance();
}
InterruptLaserDetector* InterruptLaserDetector::getInstance() {
	return InterruptLaserDetector::interruptDetector;
}
void InterruptLaserDetector::init() {
	normalDetector->init();
	startTimer(TC1, 2, TC5_IRQn, frequency);
}
void InterruptLaserDetector::update() {
	TC_GetStatus(tc, channel);
	if (state) {
		INTERRUPT_TIMING_WRITE_D(HIGH);
		setTimerSpeed(tc, channel, irq, 1 / waitTime);
		normalDetector->MeasureValueStart();
		INTERRUPT_TIMING_WRITE_D(LOW);
	}
	else {
		INTERRUPT_TIMING_WRITE_E(HIGH);
		setTimerSpeed(tc, channel, irq, 1 / (1 / frequency - waitTime));
		volatile uint16_t* measured = normalDetector->MeasureValueFinish();
		for (uint8_t i = 0; i < normalDetector->photoDiodeCount; i++) {
			valuesFiltered[i] = filter[i]->compute(measured[i]);
		}
		INTERRUPT_TIMING_WRITE_E(LOW);
	}
	state = !state;
}