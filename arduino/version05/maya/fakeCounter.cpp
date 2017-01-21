#include "fakeCounter.h"
#include "debug.h"

FakeCounter::FakeCounter(void(*notifyFunction)())
	:PositionerCounter(notifyFunction){
}

void FakeCounter::init() {
	pos = 0;
}

void FakeCounter::update() {
	pos++;
	step();
}
