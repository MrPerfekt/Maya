#include "remotePositionController.h"
#include "debug.h"
#include "config.h"

RemotePositionController::RemotePositionController(StepperWatcher* stepperWatcherH, StepperWatcher* stepperWatcherV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector, uint8_t lightBarrierCount, const uint8_t* lightBarrierPins, const uint8_t* lightBarrierOutputPins)
	:PositionController(stepperWatcherH->stepper, stepperWatcherV->stepper, discPositioner, laserDetector), stepperWatcherH(stepperWatcherH), stepperWatcherV(stepperWatcherV), lightBarrierCount(lightBarrierCount), lightBarrierPins(lightBarrierPins), lightBarrierOutputPins(lightBarrierOutputPins) {}

RemotePositionController* RemotePositionController::getInstance() {
	return (RemotePositionController*)PositionController::getInstance();
}
RemotePositionController* RemotePositionController::initInstance(StepperWatcher* stepperWatcherH, StepperWatcher* stepperWatcherV, DiscPositioner* discPositioner, InterruptLaserDetector* laserDetector, uint8_t lightBarrierCount, const uint8_t* lightBarrierPins, const uint8_t* lightBarrierOutputPins) {
	if (instance == NULL) {
		instance = new RemotePositionController(stepperWatcherH, stepperWatcherV, discPositioner, laserDetector, lightBarrierCount, lightBarrierPins, lightBarrierOutputPins);
	}
	return getInstance();
}

void remoteControllerLightBarrierStepperPinChanged() {
	RemotePositionController::getInstance()->stepperPinChanged();
}

void remoteControllerLightBarrierEncoderPinChanged() {
	RemotePositionController::getInstance()->encoderPinChanged();
}

void remoteControllerLaserDetectorAvailable() {
	RemotePositionController::getInstance()->laserDetectorAvailable();
}

void RemotePositionController::init() {
	attachInterrupt(lightBarrierPins[lightBarrierEncoderNr0], remoteControllerLightBarrierEncoderPinChanged, CHANGE);//Need to call update!
	attachInterrupt(lightBarrierPins[lightBarrierEncoderNr1], remoteControllerLightBarrierEncoderPinChanged, CHANGE);
	attachInterrupt(lightBarrierPins[lightBarrierStepperNrH], remoteControllerLightBarrierStepperPinChanged, CHANGE);
	attachInterrupt(lightBarrierPins[lightBarrierStepperNrV], remoteControllerLightBarrierStepperPinChanged, CHANGE);
	laserDetector->notifyFunction = remoteControllerLaserDetectorAvailable;
	Serial1.begin(460800);
}

void RemotePositionController::update() {
	if (laserValuesAvailable) {
		for (uint8_t i = 0; i < 4; i++) { //[0-1023] * 64
			//Serial1.print((uint16_t)(min(laserDetector->valuesFiltered[i] * (UINT16_MAX / 1024 + 1), UINT16_MAX - 1)));
			Serial1.print((uint16_t)(laserDetector->measuredValues[i]));
		}
		Serial1.print((uint16_t)UINT16_MAX);
		laserValuesAvailable = false;
	}
}

void RemotePositionController::stepperPinChanged() {
	laserDetector->setRunning(digitalRead(lightBarrierPins[lightBarrierStepperNrH]));//&& digitalRead(lightBarrierPins[lightBarrierStepperNrV]));
	lightBarrierPinChanged();
}

void RemotePositionController::encoderPinChanged() {
	for (uint8_t i = 0; i < lightBarrierCount; i++) {
		digitalWrite(lightBarrierOutputPins[i], digitalRead(lightBarrierPins[i]));
	}
	discPositioner->positionerCounter->update();
	lightBarrierPinChanged();
}

void RemotePositionController::lightBarrierPinChanged() {
	for (uint8_t i = 0; i < lightBarrierCount; i++) {
		digitalWrite(lightBarrierOutputPins[i], digitalRead(lightBarrierPins[i]));
	}
}

void RemotePositionController::laserDetectorAvailable() {
	if (laserValuesAvailable) {
		missedCount++;
	}
	laserValuesAvailable = true;
}