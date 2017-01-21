#include "encoder.h"
#include "debug.h"

uint8_t Encoder::encoderCount = 0;
Encoder *Encoder::allEncoder[Encoder::maxEncoderCount] = { 0,0 };
const int8_t Encoder::encoderFsm[4][4] = { {0,1,-1,0},{-1,0,0,1},{1,0,0,-1},{0,-1,1,0} };

void EncoderPinChanged() {
	PIN_INTERRUPT_TIMING_WRITE_B(HIGH);
	for (uint8_t i = 0; i < Encoder::encoderCount; i++) {
		Encoder::allEncoder[i]->update();
	}
	PIN_INTERRUPT_TIMING_WRITE_B(LOW);
}

Encoder::Encoder(const uint8_t pins[2], void(*notifyFunction)())
	:PositionerCounter(notifyFunction), pins(pins) {
}

void Encoder::init() {
	if (encoderCount < maxEncoderCount) {
		allEncoder[encoderCount] = this;
		encoderCount++;
		for (uint8_t i = 0; i < 3; i++) {
			attachInterrupt(pins[i], EncoderPinChanged, CHANGE);
		}
		pos = 0;
	}
}

void Encoder::update() {
	uint8_t ina = digitalRead(pins[0]) == HIGH;
	uint8_t inb = digitalRead(pins[1]) == HIGH;
	uint8_t lasta = pinStatus[0];
	uint8_t lastb = pinStatus[1];
	if (ina != lasta || inb != lastb) {
		int8_t deltaPos = encoderFsm[(ina << 1) + inb][(lasta << 1) + lastb];
		pos += deltaPos;
		direction = deltaPos == 1;
		step(direction ? 0 : 1);
		pinStatus[0] = ina;
		pinStatus[1] = inb;
	}
}