#include "remotePositionController.h"
#include "debug.h"

RemotePositionController::RemotePositionController(StepperWatcher* stepperWatcherH, StepperWatcher* stepperWatcherV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector)
	:PositionController(stepperWatcherH->stepper, stepperWatcherV->stepper, discPositioner, laserDetector), stepperWatcherH(stepperWatcherH), stepperWatcherV(stepperWatcherV){
}

RemotePositionController* RemotePositionController::getInstance() {
	return (RemotePositionController*)PositionController::getInstance();
}
RemotePositionController* RemotePositionController::initInstance(StepperWatcher* stepperWatcherH, StepperWatcher* stepperWatcherV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector) {
	if (instance == NULL) {
		instance = new RemotePositionController(stepperWatcherH, stepperWatcherV, discPositioner, laserDetector);
	}
	return getInstance();
}

void RemotePositionController::init() {
}

void RemotePositionController::update() {
}