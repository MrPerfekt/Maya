#ifndef APPROXIMATION_POSITION_CONTROLLER
#define APPROXIMATION_POSITION_CONTROLLER

#include "positionController.h"
#include "interruptLaserDetector.h"

class ApproximationPositionController : public PositionController {
	const uint8_t valuesH = 0;
	const uint8_t valuesV = 1;
	const uint8_t valuesCnt = 3;
	const int32_t encoderPosValues[3][2] = {{0,0},{0,0},{0,0}};
	ApproximationPositionController(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner);
public:
	static ApproximationPositionController* getInstance();
	static ApproximationPositionController* initInstance(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner);
	void update();
};

#endif
