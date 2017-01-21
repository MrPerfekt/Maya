#ifndef DIODE_POSITION_CONTROLLER
#define DIODE_POSITION_CONTROLLER

#include "positionController.h"
#include "interruptLaserDetector.h"

class DiodePositionController : public PositionController{
	static const int8_t speedH = 1;
	static const int8_t speedV = 1;
	double stepperPosV = 0;
	double stepperPosH = 0;
	double lowHor = 0;
	double lowVer = 0;
	uint8_t verDir = 0;
	uint32_t loopCnt = 0;
	DiodePositionController(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector);
public:
	static DiodePositionController* getInstance();
	static DiodePositionController* initInstance(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector);
	void update();
};

#endif
