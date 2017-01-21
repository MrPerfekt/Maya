#include "positionController.h"

PositionController* PositionController::instance;

PositionController::PositionController(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector)
	:stepperH(stepperH), stepperV(stepperV), discPositioner(discPositioner), laserDetector(laserDetector) {
}

PositionController* PositionController::getInstance() {
	return PositionController::instance;
}

void PositionController::init() {
}