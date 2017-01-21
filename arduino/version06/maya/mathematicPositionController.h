#ifndef MATHEMATIC_POSITION_CONTROLLER
#define MATHEMATIC_POSITION_CONTROLLER

#include "positionController.h"
#include "interruptLaserDetector.h"

class MathematicPositionController : public PositionController{
	const double distPendulumX = 0;
	const double distPendulumY = 15.2;
	const double distPendulumZ = 275;
	const double stepperSteppsPerRevolution = 64 * 63.68395 / 4; //Gear Ratio: 63.68395:1
	const double distMirrors = 15;
	const double diodeRadius = 80;
	MathematicPositionController(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner);
public:
	static MathematicPositionController* getInstance();
	static MathematicPositionController* initInstance(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner);
	void update();
};

#endif
