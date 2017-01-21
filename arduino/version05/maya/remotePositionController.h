#ifndef REMOTE_POSITION_CONTROLLER
#define REMOTE_POSITION_CONTROLLER

#include "positionController.h"
#include "interruptLaserDetector.h"
#include "stepperWatcher.h"

class RemotePositionController : public PositionController{
	StepperWatcher *stepperWatcherH;
	StepperWatcher *stepperWatcherV;
public:
	RemotePositionController(StepperWatcher* stepperWatcherH, StepperWatcher* stepperWatcherV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector);
	void init();
	void update();
	static RemotePositionController* getInstance();
	static RemotePositionController* initInstance(StepperWatcher* stepperWatcherH, StepperWatcher* stepperWatcherV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector);
};

#endif
