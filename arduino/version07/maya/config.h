#ifndef CONFIG
#define CONFIG

#include "Arduino.h"

//
// LightBarrier
//
static const uint8_t lightBarrierEncoderNr0 = 0;
static const uint8_t lightBarrierEncoderNr1 = 1;
static const uint8_t lightBarrierStepperNrH = 3;
static const uint8_t lightBarrierStepperNrV = 2;
static const uint8_t lightBarrierCount = 4;
static const uint8_t lightBarrierPins[lightBarrierCount] = { 42,43,44,45 };
static const uint8_t lightBarrierOutputPins[lightBarrierCount] = { 22, 24, 26, 28 };

//
// Encoder
//
#define FAKE_ENCODER 0
static const uint8_t pinEncoder[2] = { lightBarrierPins[lightBarrierEncoderNr0], lightBarrierPins[lightBarrierEncoderNr1] };

#if FAKE_ENCODER == 1
double fakeEncoderCounterFrequency = 10;
}
#endif

//
// KnobEncoder
//
static const uint8_t knobCount = 2;
static const uint8_t pinKnobEncoderButton[knobCount] = { 52, 53 };
static const uint8_t pinKnobEncoder[knobCount][2] = { { 48, 50 },{ 49, 51 } };

//
// Stepper
//
#define DO_INIT_STEPPER_POS 1
#define DISC_STEPPER_RUNNING 1
#define DISC_STEPPER_SOFT_START 1

static const int16_t stepperStartPosH = -183;
static const int16_t stepperStartPosV = -123;

static const uint16_t stepperSoftStartParts = 100;
static const uint16_t stepperSoftStartTime = 2000;
static const uint16_t stepperSoftStartStepTime = stepperSoftStartTime / stepperSoftStartParts;

static const uint8_t stepperNrH = 0;
static const uint8_t stepperNrV = 1;
static const uint8_t stepperNrS = 2;
static const double stepperFrequency01 = 800;
static const double stepperFrequency2 = 150;//200;
static const uint8_t stepperCount = 3;
static const uint8_t stepperPins[stepperCount][4] = { { 23, 27, 25, 29 },{ 31, 35, 33, 37 },{ 8, 9, 10, 11 } };
static const uint8_t stepperWatcherCount = 2;
static const uint8_t stepperWatcherInputPins[stepperWatcherCount][4] = { { 22, 24, 26, 28 },{ 30, 32, 34, 36 } };

//
// Laser
//
static const uint8_t laserPin = 41;
static const double laserDuty = 0.1;
static const double laserFrequency = 200;
static const double laserMeasureTime = 0.000001 * 50;
static const double laserLighteningTime = laserDuty / laserFrequency - laserMeasureTime;
static const double laserWaitTime = laserLighteningTime + laserMeasureTime;
//static const double laserWaitTime = laserMeasureTime+0.0001;
static const uint16_t photoDiodeFilterTimeConstant = 1;
static const uint8_t photoDiodeCount = 4;
static const uint8_t photoDiodePin[photoDiodeCount] = { A0,A1,A2,A3 };
#define PD_LEFT_UP 2
#define PD_LEFT_DOWN 0
#define PD_RIGHT_DOWN 1
#define PD_RIGHT_UP 3

//
// PositionController
//
#define POSITION_CONTROLLER_DIODE 0
#define POSITION_CONTROLLER_MATHEMATIC 1
#define POSITION_CONTROLLER_APPROXIMATION 2
#define POSITION_CONTROLLER_REMOTE 3
static const uint8_t positionControllerType = 0;

static const double encoderSteppsPerRevolution = 468;//117*4
static const bool encoderUseTimeCorrection = false;
static const uint8_t encoderBigMarkSize = 3;
static const uint8_t encoderSteppDistance = 7;

static const uint32_t stepperWatcherMinStepTime = (uint32_t)(1000000/(stepperFrequency01+100)); //In microseconds


#endif