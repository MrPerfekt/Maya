#include "counter.h"
#include "debug.h"

uint8_t Counter::counterCount = 0;
Counter *Counter::allCounter[Counter::maxCounterCount] = { 0 };

void CounterPinChanged() {
	PIN_INTERRUPT_TIMING_WRITE_A(HIGH);
	for (uint8_t i = 0; i < Counter::counterCount; i++) {
		Counter::allCounter[i]->update();
	}
	PIN_INTERRUPT_TIMING_WRITE_A(LOW);
}

Counter::Counter(uint8_t pin, void(*notifyFunction)())
	:PositionerCounter(notifyFunction), pin(pin) {
}

void Counter::init() {
	if (counterCount < maxCounterCount) {
		allCounter[counterCount] = this;
		counterCount++;
		for (uint8_t i = 0; i < 3; i++) {
			attachInterrupt(pin, CounterPinChanged, CHANGE);
		}
		pos = 0;
	}
}

void Counter::update() {
	uint8_t in = digitalRead(pin) == HIGH;
	if (in != pinStatus) {
		pos++;
		pinStatus = in;
		step();
	}
}
