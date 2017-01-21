#include "differenceIntensityLaserDetector.h"

DifferenceIntensityLaserDetector::DifferenceIntensityLaserDetector(uint8_t photoDiodeCount, uint8_t pinLaser, const uint8_t pinPhotoDiode[])
	:photoDiodeValueAfter(new uint16_t[photoDiodeCount]), photoDiodeValueBefore(new uint16_t[photoDiodeCount]), photoDiodeCount(photoDiodeCount), pinLaser(pinLaser), pinPhotoDiode(pinPhotoDiode) {
}

void DifferenceIntensityLaserDetector::init() {
	pinMode(pinLaser, OUTPUT);
}

void DifferenceIntensityLaserDetector::MeasureValue() {
	//digitalWrite(pinLaser,LOW);
	//delayMicroseconds(2);
	MeasureValueStart();
	delayMicroseconds(50);
	MeasureValueFinish();
}

void DifferenceIntensityLaserDetector::MeasureValueStart() {
	for (uint8_t i = 0; i < photoDiodeCount; i++) {
		photoDiodeValueBefore[i] = analogRead(pinPhotoDiode[i]);
	}
	digitalWrite(pinLaser, HIGH);
}

volatile uint16_t* DifferenceIntensityLaserDetector::MeasureValueFinish() {
	for (uint8_t i = 0; i < photoDiodeCount; i++) {
		uint16_t current = analogRead(pinPhotoDiode[i]);
		//photoDiodeValueAfter[i] = analogRead(pinPhotoDiode[i]) - photoDiodeValueBefore[i];
		if (current > photoDiodeValueBefore[i]) {
			photoDiodeValueAfter[i] = current - photoDiodeValueBefore[i];
		}
		else {
			photoDiodeValueAfter[i] = 0;
		}
	}
	digitalWrite(pinLaser, LOW);
	return photoDiodeValueAfter;
}
