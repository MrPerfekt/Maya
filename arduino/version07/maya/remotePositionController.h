#ifndef REMOTE_POSITION_CONTROLLER
#define REMOTE_POSITION_CONTROLLER

#include "positionController.h"
#include "interruptLaserDetector.h"
#include "stepperWatcher.h"

class RemotePositionController : public PositionController{
	StepperWatcher *stepperWatcherH;
	StepperWatcher *stepperWatcherV;
	const uint8_t* lightBarrierPins;
	const uint8_t* lightBarrierOutputPins;
	const uint8_t lightBarrierCount;
	bool laserValuesAvailable;
	uint16_t missedCount = 0;
public:
	RemotePositionController(StepperWatcher* stepperWatcherH, StepperWatcher* stepperWatcherV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector, uint8_t lightBarrierCount, const uint8_t* lightBarrierPins, const uint8_t* lightBarrierOutputPins);
	void init();
	void update();
	void stepperPinChanged();
	void encoderPinChanged();
	void lightBarrierPinChanged();
	void laserDetectorAvailable();
	static RemotePositionController* getInstance();
	static RemotePositionController* initInstance(StepperWatcher* stepperWatcherH, StepperWatcher* stepperWatcherV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector, uint8_t lightBarrierCount, const uint8_t* lightBarrierPins, const uint8_t* lightBarrierOutputPins);
};

#endif
