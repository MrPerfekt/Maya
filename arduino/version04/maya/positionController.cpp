#include "positionController.h"

PositionController::PositionController(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector)
	:stepperH(stepperH), stepperV(stepperV), discPositioner(discPositioner), laserDetector(laserDetector) {
}