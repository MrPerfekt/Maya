#include "diodePositionController.h"
#include "debug.h"

DiodePositionController::DiodePositionController(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector)
	:PositionController(stepperH, stepperV, discPositioner, laserDetector){
}
void DiodePositionController::update() {
	DEBUG_OUT(HIGH);
	loopCnt++;
	// 1 4
	// 2 3
	#define PD_LEFT_UP 0
	#define PD_LEFT_DOWN 1
	#define PD_RIGHT_DOWN 2
	#define PD_RIGHT_UP 3
	volatile double* diodeValues = laserDetector->valuesFiltered;
	double diodeLU = diodeValues[PD_LEFT_UP];
	double diodeLD = diodeValues[PD_LEFT_DOWN];
	double diodeRU = diodeValues[PD_RIGHT_UP];
	double diodeRD = diodeValues[PD_RIGHT_DOWN];
	double horizontal = diodeLU + diodeLD - diodeRU - diodeRD;
	double vertical = diodeLU + diodeRU - diodeLD - diodeRD;

	lowHor = (lowHor*lowPassGrade + horizontal) / (lowPassGrade + 1);
	lowVer = (lowVer*lowPassGrade + vertical) / (lowPassGrade + 1);

	if (loopCnt % 50 == 0) {
		double encoderAngle = discPositioner->getAngle() * PI / 180;
		double sinV = sin(encoderAngle);
		double cosV = cos(encoderAngle);
		if (lowVer > triggerLevel) {
			stepperPosV += cosV;
			stepperPosH -= sinV;
		}
		else if (lowVer < -triggerLevel) {
			stepperPosV -= cosV;
			stepperPosH += sinV;
		}
		if (lowHor > triggerLevel) {
			stepperPosH += cosV;
			stepperPosV += sinV;
		}
		else if (lowHor < -triggerLevel) {
			stepperPosH -= cosV;
			stepperPosV -= sinV;
		}
		stepperV->pos = stepperPosV;
		stepperH->pos = stepperPosH;
	}
	DEBUG_OUT(LOW);
}

DiodePositionController* DiodePositionController::getInstance() {
	return (DiodePositionController*)PositionController::getInstance();
}
DiodePositionController* DiodePositionController::initInstance(Stepper* stepperH, Stepper* stepperV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector) {
	if (instance == NULL) {
		instance = new DiodePositionController(stepperH,stepperV,discPositioner,laserDetector);
	}
	return getInstance();
}
