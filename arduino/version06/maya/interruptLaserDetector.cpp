#include "interruptLaserDetector.h"
#include "timer.h"
#include "debug.h"

uint16_t* TESTmeasured = { 0 };
InterruptLaserDetector* InterruptLaserDetector::interruptDetector;

void TC5_Handler() {
	TIMER_INTERRUPT_TIMING_WRITE_C(HIGH);
	InterruptLaserDetector::getInstance()->update();
	TIMER_INTERRUPT_TIMING_WRITE_C(LOW);
}

InterruptLaserDetector::InterruptLaserDetector(DifferenceIntensityLaserDetector* normalDetector, Filter<double>** filter, double frequency, double waitTime, void(*notifyFunction)(), bool useFilter)
	:measuredValues(new uint16_t[normalDetector->photoDiodeCount]), valuesFiltered(new double[normalDetector->photoDiodeCount]), normalDetector(normalDetector), filter(filter), frequencyMeasure((uint32_t)(1 / waitTime)), frequencySleep((uint32_t)(1 / (1 / frequency - waitTime))), notifyFunction(notifyFunction), useFilter(useFilter) {
	for (uint8_t i = 0; i < normalDetector->photoDiodeCount; i++){
		valuesFiltered[i] = 0;
	}
}
InterruptLaserDetector* InterruptLaserDetector::initInstance(DifferenceIntensityLaserDetector* normalDetector, Filter<double>** filter, double frequency, double waitTime, void(*notifyFunction)(), bool useFilter) {
	if (interruptDetector == NULL) {
		interruptDetector = new InterruptLaserDetector(normalDetector, filter, frequency, waitTime,notifyFunction, useFilter);
	}
	return getInstance();
}
InterruptLaserDetector* InterruptLaserDetector::getInstance() {
	return InterruptLaserDetector::interruptDetector;
}
void InterruptLaserDetector::init() {
	normalDetector->init();
	startTimer(tc, channel, irq, frequencySleep);
}
void InterruptLaserDetector::update() {
	TC_GetStatus(tc, channel);
	if (running) {
		if (state) {
			TIMER_INTERRUPT_TIMING_WRITE_D(HIGH);
			setTimerSpeed(tc, channel, frequencyMeasure);
			normalDetector->MeasureValueStart();
			TIMER_INTERRUPT_TIMING_WRITE_D(LOW);
		} else {
			TIMER_INTERRUPT_TIMING_WRITE_E(HIGH);
			setTimerSpeed(tc, channel, frequencySleep);
			measuredValues = normalDetector->MeasureValueFinish();
			if (useFilter) {
				for (uint8_t i = 0; i < normalDetector->photoDiodeCount; i++) {
					valuesFiltered[i] = filter[i]->compute(measuredValues[i]);
				}
			}
			notify();
			TIMER_INTERRUPT_TIMING_WRITE_E(LOW);
		}
		state = !state;
	}
}

void InterruptLaserDetector::setRunning(bool run) {
	running = run;
	if (run == false) {
		digitalWrite(normalDetector->pinLaser, LOW);
	}
}

void InterruptLaserDetector::notify() {
	if (notifyFunction != 0) {
		notifyFunction();
	}
}