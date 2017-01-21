#include "mathematicPositionController.h"
#include "debug.h"

MathematicPositionController::MathematicPositionController(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner)
	:PositionController(stepperH, stepperV, discPositioner,0) {
}

void MathematicPositionController::update() {
	double encoderAngle = discPositioner->getAngle() * PI / 180;
	double posX = diodeRadius*sin(encoderAngle) + distPendulumX;
	double posY = diodeRadius*(cos(encoderAngle) - 1) + distPendulumY;
	int32_t stepperPosH = (int32_t)(stepperSteppsPerRevolution*asin(posX / (distPendulumZ + distMirrors)) / (2 * PI));
	int32_t stepperPosV = (int32_t)(stepperSteppsPerRevolution*asin(posY / distPendulumZ) / (2 * PI));
	stepperH->pos = stepperPosH;
	stepperV->pos = stepperPosV;
}

MathematicPositionController* MathematicPositionController::getInstance() {
	return (MathematicPositionController*)PositionController::getInstance();
}
MathematicPositionController* MathematicPositionController::initInstance(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner) {
	if (instance == NULL) {
		instance = new MathematicPositionController(stepperH, stepperV, discPositioner);
	}
	return getInstance();
}
