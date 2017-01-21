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
static const uint8_t lightBarrierPins[lightBarrierCount] = { 51,53,49,47 };
static const uint8_t lightBarrierOutputPins[lightBarrierCount] = { 3, 4, 5, 6 };

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
// 5-Way-Switch
//
static const uint8_t switchCnt = 5;
static const uint8_t switchUp = 2;
static const uint8_t switchDown = 4;
static const uint8_t switchRight = 0;
static const uint8_t switchLeft = 3;
static const uint8_t switchMiddle = 1;
static const uint8_t pinSwitch[switchCnt] = { 33,35,37,39,41 };

//
// KnobEncoder
//
static const uint8_t knobCount = 3;
static const uint8_t pinKnobEncoderButton[knobCount] = { DAC0, A11, 44 };
static const uint8_t pinKnobEncoder[knobCount][2] = { { CANRX, DAC1 },{ A5, A4 },{40,42} };

//
// Stepper
//
#define DO_INIT_STEPPER_POS 1
#define DISC_STEPPER_RUNNING 1
#define DISC_STEPPER_SOFT_START 1

static const int16_t stepperStartPosH = 170;
static const int16_t stepperStartPosV = 122;

static const uint16_t stepperSoftStartParts = 100;
static const uint16_t stepperSoftStartTime = 2000;
static const uint16_t stepperSoftStartStepTime = stepperSoftStartTime / stepperSoftStartParts;

static const uint8_t stepperNrH = 0;
static const uint8_t stepperNrV = 1;
static const uint8_t stepperNrS = 2;
static const uint16_t stepperFrequency01 = 800;//800;
static const uint16_t stepperStartFrequency2 = 100;
static const uint16_t stepperMaxFrequency2 = 200;// 200;// 300;
static const int16_t stepperSoftSpeedStep = 10;
static const uint16_t driveStepperStepTimeChangeDivider = 500;
static const uint8_t stepperCount = 3;
static const uint8_t stepperPins[stepperCount][4] = { { 23, 27, 25, 29 },{18, 20, 19, 21},{ SCL1, SDA1, 13, 12 } };
static const uint8_t stepperWatcherCount = 2;
static const uint8_t stepperWatcherInputPins[stepperWatcherCount][4] = { { 24, 26, 28, 30 },{ 32, 34, 36, 38 } };

//
// Laser
//
static const uint8_t laserPin = A7;
static const double laserDuty = 0.1;
static const double laserFrequency = 200;
static const double laserMeasureTime = 0.000001 * 50;
static const double laserLighteningTime = laserDuty / laserFrequency - laserMeasureTime;
static const double laserWaitTime = laserLighteningTime + laserMeasureTime;
//static const double laserWaitTime = laserMeasureTime+0.0001;
static const uint16_t photoDiodeFilterTimeConstant = 1;
static const uint8_t photoDiodeCount = 4;
static const uint8_t photoDiodePin[photoDiodeCount] = { A0,A1,A2,A3 };
#define PD_LEFT_UP 1
#define PD_LEFT_DOWN 0
#define PD_RIGHT_DOWN 2
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
static const uint32_t mainTimeDiff = 110;//us

//
// DiodePositionController
//
static const uint16_t diodePositionControllerLoopDivider = 25; //12== (uint16_t)(1000000.0/((double)stepperFrequency01*mainTimeDiff)); //stepperfrequency == (mainTimeDiff * loopDivider)^(-1)
static const uint16_t diodePositionControllerTriggerLevel = 200;
static const uint16_t diodePositionControllerLowPassGrade = 2;

#endif