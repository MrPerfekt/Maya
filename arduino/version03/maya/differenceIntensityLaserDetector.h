#ifndef DIFFERENCE_INTENSITY_LASER_DETECTOR
#define DIFFERENCE_INTENSITY_LASER_DETECTOR

#include "Arduino.h"

class DifferenceIntensityLaserDetector {
  uint8_t photoDiodeCount;
  uint8_t pinLaser;
  const uint8_t *pinPhotoDiode;
  public:
  DifferenceIntensityLaserDetector(uint8_t photoDiodeCount, uint8_t pinLaser, const uint8_t pinPhotoDiode[]);
  void MeasureValue(uint16_t *intensity);
};

#endif
