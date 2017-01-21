#include "diodePositionController.h"
#include "debug.h"
#include "config.h"

DiodePositionController::DiodePositionController(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector)
	:PositionController(stepperH, stepperV, discPositioner, laserDetector) {}
void DiodePositionController::update() {
	loopCnt++;
	// 1 4
	// 2 3
	volatile uint16_t* diodeValues = laserDetector->measuredValues;
	uint16_t diodeLU = diodeValues[PD_LEFT_UP];
	uint16_t diodeLD = diodeValues[PD_LEFT_DOWN];
	uint16_t diodeRU = diodeValues[PD_RIGHT_UP];
	uint16_t diodeRD = diodeValues[PD_RIGHT_DOWN];
	//int16_t horizontal = diodeLU + diodeLD - diodeRU - diodeRD;
	//int16_t vertical = diodeLU + diodeRU - diodeLD - diodeRD;
	int16_t horizontal = max(diodeLU, diodeLD) - max(diodeRU, diodeRD);
	int16_t vertical = max(diodeLU, diodeRU) - max(diodeLD, diodeRD);

	lowHor = (lowHor*diodePositionControllerLowPassGrade + horizontal) / (diodePositionControllerLowPassGrade + 1);
	lowVer = (lowVer*diodePositionControllerLowPassGrade + vertical) / (diodePositionControllerLowPassGrade + 1);

	if (loopCnt % diodePositionControllerLoopDivider == 0) {
		double encoderAngle = discPositioner->getAngle() * PI / 180;
		double sinV = sin(encoderAngle)*speedH;
		double cosV = cos(encoderAngle)*speedV;
		if (lowVer > diodePositionControllerTriggerLevel) {
			stepperPosV -= cosV;
			stepperPosH += sinV;
		} else if (lowVer < -diodePositionControllerTriggerLevel) {
			stepperPosV += cosV;
			stepperPosH -= sinV;
		}
		if (lowHor > diodePositionControllerTriggerLevel) {
			stepperPosH += cosV;
			stepperPosV += sinV;
		} else if (lowHor < -diodePositionControllerTriggerLevel) {
			stepperPosH -= cosV;
			stepperPosV -= sinV;
		}
		stepperV->pos = stepperPosV;
		stepperH->pos = stepperPosH;
	}
}

DiodePositionController* DiodePositionController::getInstance() {
	return (DiodePositionController*)PositionController::getInstance();
}
DiodePositionController* DiodePositionController::initInstance(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector) {
	if (instance == NULL) {
		instance = new DiodePositionController(stepperH, stepperV, discPositioner, laserDetector);
	}
	return getInstance();
}
