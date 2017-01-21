#ifndef POSITION_CONTROLLER
#define POSITION_CONTROLLER

#include "stepper.h"
#include "interruptLaserDetector.h"
#include "discPositioner.h"

class PositionController{
protected:
	Stepper* stepperH;
	Stepper* stepperV;
	DiscPositioner* discPositioner;
	InterruptLaserDetector *laserDetector;
	PositionController(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector);
public:
	static PositionController* instance;
	static PositionController* getInstance();
	virtual void update() = 0;
	virtual void init();
};

#endif
