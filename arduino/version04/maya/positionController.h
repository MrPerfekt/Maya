#ifndef POSITION_CONTROLLER
#define POSITION_CONTROLLER

#include "stepper.h"
#include "interruptLaserDetector.h"
#include "discPositioner.h"

class PositionController{
protected:
	Stepper* stepperH;
	Stepper* stepperV;
	InterruptLaserDetector* laserDetector;
	DiscPositioner* discPositioner;
public:
	PositionController(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector);
	virtual void update() = 0;
};

#endif
