#include "approximationPositionController.h"
#include "debug.h"

ApproximationPositionController::ApproximationPositionController(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner)
	:PositionController(stepperH, stepperV, discPositioner,0) {
}
void ApproximationPositionController::update() {
	int32_t discAngle = discPositioner->getAngle();
	uint8_t index0 = discAngle / valuesCnt;
	uint8_t index1 = index0 + 1;
	int32_t deltaPos = discAngle % valuesCnt;

	int32_t stepperBaseH = encoderPosValues[index0][valuesH];
	int32_t stepperBaseV = encoderPosValues[index0][valuesV];

	int32_t stepperRiseH = encoderPosValues[index1][valuesH] - stepperBaseH;
	int32_t stepperRiseV = encoderPosValues[index1][valuesV] - stepperBaseV;

	stepperH->pos = stepperBaseH + (stepperRiseH * deltaPos) / 20;
	stepperV->pos = stepperBaseV + (stepperRiseV * deltaPos) / 20;
}

ApproximationPositionController* ApproximationPositionController::getInstance() {
	return (ApproximationPositionController*)PositionController::getInstance();
}
ApproximationPositionController* ApproximationPositionController::initInstance(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner) {
	if (instance == NULL) {
		instance = new ApproximationPositionController(stepperH, stepperV, discPositioner);
	}
	return getInstance();
}