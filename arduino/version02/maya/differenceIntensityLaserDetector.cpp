#include "differenceIntensityLaserDetector.h"

DifferenceIntensityLaserDetector::DifferenceIntensityLaserDetector(uint8_t photoDiodeCount, uint8_t pinLaser, const uint8_t pinPhotoDiode[])
:photoDiodeCount(photoDiodeCount), pinLaser(pinLaser), pinPhotoDiode(pinPhotoDiode) {
  pinMode(pinLaser, OUTPUT);
}
void DifferenceIntensityLaserDetector::MeasureValue(uint16_t *intensity) {
  uint16_t photoDiodeValueBefore[photoDiodeCount];

  //digitalWrite(pinLaser,LOW);
  //delayMicroseconds(2);
  //digitalWrite(debugPin,LOW);
  for (uint8_t i = 0; i < photoDiodeCount; i++) {
    photoDiodeValueBefore[i] = analogRead(pinPhotoDiode[i]);
  }
  digitalWrite(pinLaser, HIGH);
  delayMicroseconds(50);
  //digitalWrite(debugPin,HIGH);
  for (uint8_t i = 0; i < photoDiodeCount; i++) {
    uint16_t photoDiodeValueAfter = analogRead(pinPhotoDiode[i]);
    intensity[i] = photoDiodeValueAfter - photoDiodeValueBefore[i];
  }
  digitalWrite(pinLaser, LOW);
  //analogWrite(pinLaser,127);
}
