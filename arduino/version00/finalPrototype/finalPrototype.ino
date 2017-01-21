//#define APPROXIMATION
//#define DIODE_SEARCH
#define MATHEMATIC

//==Laser==
const uint8_t pinLaser = 11;

//==LEDs==
const uint8_t ledCnt = 9;
const uint8_t ledPins[ledCnt]={43,45,47,49,51,53,48,50,52};

//==Encoder==
const int8_t fsmEncoder[4][4]={{0,1,-1,0},{-1,0,0,1},{1,0,0,-1},{0,-1,1,0}};
const uint8_t pinEncoderA=2;
const uint8_t pinEncoderB=3;
unsigned long lastEncoderTime = 0;
unsigned long lastEncoderTimeDiff;
int encoderPos=0;

//==KnobEncoder==
const uint8_t knobCnt = 2;
const uint8_t knobEncoderPin[knobCnt][2]={{28,26},{24,22}};
const uint8_t knobEncoderButtonPin[knobCnt]={25,27};
uint8_t knobEncoderLastState[knobCnt][2]={{0,0},{0,0}};
int32_t knobEncoderPos[knobCnt];

//==Stepper==
const int stepperPinCnt = 4;
const int stepperStateCnt = 8;
const int stepperPins[2][stepperPinCnt]={{30,32,34,36},{31,33,35,37}};
const uint8_t fsmStepper[stepperStateCnt][stepperPinCnt]={
  {1,0,0,0},
  {1,1,0,0},
  {0,1,0,0},
  {0,1,1,0},
  {0,0,1,0},
  {0,0,1,1},
  {0,0,0,1},
  {1,0,0,1}
};
volatile int32_t stepperPos[2]={0,0};
volatile int32_t stepperPosDelta[2]={0,0};
volatile int32_t stepperPosReal[2]={0,0};
#define STEPPER_H 0
#define STEPPER_V 1

//==Magnet==
const uint8_t pinMagnet[2] = {46,44};
const uint8_t pinMagnetPwm = 10;
const uint8_t range = 20;

//==PhotoDiode==
const uint8_t photoDiodeCnt = 4;
const uint8_t photoDiodePin[photoDiodeCnt] = {A0,A1,A2,A3};
uint16_t photoDiodeValueBefore[photoDiodeCnt] = {0,0,0,0};
uint16_t photoDiodeValueAfter[photoDiodeCnt] = {0,0,0,0};
// 1 4
// 2 3
#define PD_LEFT_UP 0
#define PD_LEFT_DOWN 1
#define PD_RIGHT_DOWN 2
#define PD_RIGHT_UP 3

//==DebugPin==
const uint8_t debugPin = 23;

//==IRQ==
void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
        pmc_set_writeprotect(false);
        pmc_enable_periph_clk((uint32_t)irq);
        TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
        uint32_t rc = VARIANT_MCK/128/frequency; //128 because we selected TIMER_CLOCK4 above
        TC_SetRA(tc, channel, rc/2); //50% high, 50% low
        TC_SetRC(tc, channel, rc);
        TC_Start(tc, channel);
        tc->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;
        tc->TC_CHANNEL[channel].TC_IDR=~TC_IER_CPCS;
        NVIC_EnableIRQ(irq);
}
/*
ISR/IRQ TC          Channel Due pins
TC0 TC0 0 2, 13
TC1 TC0 1 60, 61
TC2 TC0 2 58
TC3 TC1 0 none
TC4 TC1 1 none
TC5 TC1 2 none
TC6 TC2 0 4, 5
TC7 TC2 1 3, 10
TC8 TC2 2 11, 12
 */
  
void setup() {  
  //==DebugPin==
  pinMode(debugPin,OUTPUT);
  
  //==Laser==
  pinMode(pinLaser,OUTPUT);
  
  //==LEDs==
  for(uint8_t i = 0; i < ledCnt;i++){
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  
  //==Magnet==
  pinMode(pinMagnet[0], OUTPUT);
  pinMode(pinMagnet[1], OUTPUT);
  pinMode(pinMagnetPwm, OUTPUT);

  //==Serial==
  Serial.begin(9600);
  
  //==Encoder==
  pinMode(pinEncoderA, INPUT);
  pinMode(pinEncoderB, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinEncoderA), incrementEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinEncoderB), incrementEncoder, CHANGE);
  
  //==Stepper==
  for(int i = 0; i < stepperPinCnt; i++){
    pinMode(stepperPins[0][i], OUTPUT);
    pinMode(stepperPins[1][i], OUTPUT);
  }
  startTimer(TC1, 0, TC3_IRQn, 900);
}

// -120, -100, -80, -60, -40, -20, 0, 20, 40, 60, 80, 100
#define VALUES_V 0
#define VALUES_H 1
const int32_t encoderPosValues[12][2] = {{-20,68},{-15,57},{-10,46},{-7,34},{-4,22},{-2,11},{-3,0},{-4,-13},{-8,-26},{-12,-38},{-17,-48},{-23,-60}};
uint32_t loopCnt = 0;
void loop(){
  loopCnt++;

  /*
  //Stepper Speed Test
  //900-1200
  stepperPosDelta[STEPPER_V] = 100000;
  updateKnobEncoder(0);
  if(knobEncoderPos[0] != 0){
    static uint32_t frequency = 900;
    frequency += knobEncoderPos[0];
    knobEncoderPos[0] = 0;
    startTimer(TC1, 0, TC3_IRQn, frequency);
    Serial.println(frequency);
  }
  //updateStepper(STEPPER_V);
  delay(1);
  return;
  */
    
  if(loopCnt % 10 == 0){
    updateKnobEncoder(0);
    updateKnobEncoder(1);
    stepperPosDelta[STEPPER_H] += knobEncoderPos[0];
    stepperPosDelta[STEPPER_V] += knobEncoderPos[1];
    knobEncoderPos[0] = 0;
    knobEncoderPos[1] = 0;
  }

  if(digitalRead(knobEncoderButtonPin[0])==LOW){
  }
  if(digitalRead(knobEncoderButtonPin[1])==LOW){
  }
  
  readPhotodiodes();

#ifdef DIODE_SEARCH
    int16_t diodeLU = max(0,(int16_t)photoDiodeValueAfter[PD_LEFT_UP]-(int16_t)photoDiodeValueBefore[PD_LEFT_UP]);
    int16_t diodeLD = max(0,(int16_t)photoDiodeValueAfter[PD_LEFT_DOWN]-(int16_t)photoDiodeValueBefore[PD_LEFT_DOWN]);
    int16_t diodeRU = max(0,(int16_t)photoDiodeValueAfter[PD_RIGHT_UP]-(int16_t)photoDiodeValueBefore[PD_RIGHT_UP]);
    int16_t diodeRD = max(0,(int16_t)photoDiodeValueAfter[PD_RIGHT_DOWN]-(int16_t)photoDiodeValueBefore[PD_RIGHT_DOWN]);
    int16_t horizontal = diodeLU+diodeLD-diodeRU-diodeRD;
    int16_t vertical = diodeLU+diodeRU-diodeLD-diodeRD;
    
    static double lowHor = 0;
    const uint8_t lowPassGrade = 10;
    const uint8_t triggerLevel = 50;
    static uint8_t verDir = 0;
    lowHor = (lowHor*lowPassGrade+horizontal)/(lowPassGrade+1);
    static double lowVer = 0;
    lowVer = (lowVer*lowPassGrade+vertical)/(lowPassGrade+1);

    static double stepperPosV = 0;
    static double stepperPosH = 0;
    if(loopCnt % 6 == 0){
      double encoderAngle = encoderPos*PI/4/180;
      double sinV = sin(encoderAngle);
      double cosV = cos(encoderAngle);
      if(lowVer > triggerLevel){
        stepperPosV += cosV;
        stepperPosH -= sinV;
      }else if(lowVer < -triggerLevel){
        stepperPosV -= cosV;
        stepperPosH += sinV;
      }
      if(lowHor > triggerLevel){
        stepperPosH += cosV;
        stepperPosV += sinV;
      }else if(lowHor < -triggerLevel){
        stepperPosH -= cosV;
        stepperPosV -= sinV;
      }
      stepperPos[STEPPER_V] = (int)stepperPosV;
      stepperPos[STEPPER_H] = (int)stepperPosH;
    }
#endif
#ifdef APPROXIMATION
  int32_t pos = min(100,max(-119,encoderPos));
  uint8_t index0 = (pos+120)/20;
  uint8_t index1 = index0+1;
  int32_t deltaPos = (pos+120) % 20;
  
  int32_t stepperBaseH = encoderPosValues[index0][VALUES_H];
  int32_t stepperBaseV = encoderPosValues[index0][VALUES_V];
  
  int32_t stepperRiseH = encoderPosValues[index1][VALUES_H] - stepperBaseH;
  int32_t stepperRiseV = encoderPosValues[index1][VALUES_V] - stepperBaseV;

  stepperPos[STEPPER_H] = stepperBaseH + (stepperRiseH * deltaPos) / 20;
  stepperPos[STEPPER_V] = stepperBaseV + (stepperRiseV * deltaPos) / 20;
  if(loopCnt % 10000 == 0){
    Serial.print(encoderPos);
    Serial.print(" ");
    Serial.print(deltaPos);
    Serial.print(" ");
    Serial.print(stepperBaseH + (stepperRiseH * deltaPos) / 20);
    Serial.print(" ");
    Serial.println(stepperBaseV + (stepperRiseV * deltaPos) / 20);
  }
#endif
#ifdef MATHEMATIC
//  //Full Rotation = 64*64=4096
//  //Pendulum Laser Dist
//  //Dist = 30cm
//  //DistY = 2cm
//  //Pendulum Heigh 17.1CM
//  double pendulumDistX = 0;
//  double pendulumDistY = 0;
//  double pendulumDistZ = 305;
//  double pendulumLength = 171;
//  double encoderAngle = encoderPos*PI/4/180;
//  double posX = pendulumLength*sin(-encoderAngle)+pendulumDistX;
//  double posY = pendulumLength*(cos(encoderAngle)-1)+pendulumDistY;
//  stepperPos[STEPPER_H] = (int)(4096*asin(posX/(pendulumDistZ+1.5))/(2*PI)/2);
//  stepperPos[STEPPER_V] = (int)(4096*asin(posY/pendulumDistZ)/(2*PI)/2);
#endif
}


//==PhotoDiodes==
void readPhotodiodes(){
  //digitalWrite(pinLaser,LOW);
  //delayMicroseconds(2);
  digitalWrite(debugPin,LOW);
  for(int i = 0; i < photoDiodeCnt; i++){
    photoDiodeValueBefore[i] = analogRead(photoDiodePin[i]);
  }
  digitalWrite(pinLaser,HIGH);
  delayMicroseconds(50);
  digitalWrite(debugPin,HIGH);
  for(int i = 0; i < photoDiodeCnt; i++){
    photoDiodeValueAfter[i] = analogRead(photoDiodePin[i]);
  }
  digitalWrite(pinLaser,LOW);
  //analogWrite(pinLaser,127);
}

//==Magnet==
void setMagnet(bool on, bool pull, uint8_t speed){
    digitalWrite(pinMagnet[0],on && !pull);
    digitalWrite(pinMagnet[1],on && pull);
    analogWrite(pinMagnetPwm,speed);
}

//==Stepper==
void TC3_Handler() {
  TC_GetStatus(TC1, 0);
  updateStepper(STEPPER_H);
  updateStepper(STEPPER_V);
}
void updateStepper(uint8_t stepper){
  int32_t pos = stepperPos[stepper] + stepperPosDelta[stepper];
  if(pos > stepperPosReal[stepper]){
    stepperPosReal[stepper]++;
  }else if(pos < stepperPosReal[stepper]){
    stepperPosReal[stepper]--;
  }else{
    for(int i = 0; i < stepperPinCnt; i++){
      //digitalWrite(stepperPins[stepper][i],LOW);
    }
    return;
  }
  int stepperState = stepperPosReal[stepper] % stepperStateCnt;
  if(stepperState < 0){
    stepperState += stepperStateCnt;
  }
  for(int i = 0; i < stepperPinCnt; i++){
    digitalWrite(stepperPins[stepper][i],fsmStepper[stepperState][i]);
  }
}

void updateKnobEncoder(uint8_t nr){
  uint8_t lasta = knobEncoderLastState[nr][0];
  uint8_t lastb = knobEncoderLastState[nr][1];
  uint8_t ina = digitalRead(knobEncoderPin[nr][0])==HIGH;
  uint8_t inb = digitalRead(knobEncoderPin[nr][1])==HIGH;
  if(ina==lasta && inb==lastb){
    return;
  }
  knobEncoderPos[nr]+=fsmEncoder[(ina<<1)+inb][(lasta<<1)+lastb];
  knobEncoderLastState[nr][0] = ina;
  knobEncoderLastState[nr][1] = inb;
}

void incrementEncoder(){
  static uint8_t lasta = 0;
  static uint8_t lastb = 0;
  uint8_t ina = digitalRead(pinEncoderA)==HIGH;
  uint8_t inb = digitalRead(pinEncoderB)==HIGH;
  if(ina==lasta && inb==lastb){
    return;
  }
  int encoderDir = fsmEncoder[(ina<<1)+inb][(lasta<<1)+lastb];
  encoderPos+=fsmEncoder[(ina<<1)+inb][(lasta<<1)+lastb];
  lasta = ina;
  lastb = inb;
  
  unsigned long newTime = micros();
  lastEncoderTimeDiff = newTime - lastEncoderTime;
  lastEncoderTime = newTime;

  for(uint8_t i = 0; i < ledCnt;i++){
    digitalWrite(ledPins[i],(ledCnt-i)*5 > abs(encoderPos));
  }  

  static bool magnetDir = false;
  static uint8_t magnetOn = 1;
  static int maximum = 0;
  if(abs(encoderPos) > range){
    if(magnetOn){
      magnetOn = 0;
      setMagnet(false,false,0); //off
      maximum = 0;
    } else {
      if(abs(maximum) < abs(encoderPos)){
        maximum = encoderPos;
      }
    }
  } else {
    if(!magnetOn){
      magnetOn = 1;
      setMagnet(1,true,215);//176
      magnetDir = true;
    } else if(((encoderPos < 0) != (maximum < 0)) && magnetDir){
      magnetDir = false;
      setMagnet(0,false,0);
    }
  }


#ifdef MATHEMATIC
  double pendulumDistX = 0;
  double pendulumDistY = 0;
  double pendulumDistZ = 263;
  double pendulumLength = 135;
  double encoderAngle = encoderPos*PI/4/180 + encoderDir*15.0/(double)lastEncoderTimeDiff;
  double posX = pendulumLength*sin(-encoderAngle)+pendulumDistX;
  double posY = pendulumLength*(cos(encoderAngle)-1)+pendulumDistY;
  stepperPos[STEPPER_H] = (int)(4096*asin(posX/(pendulumDistZ+20))/(2*PI)/2);
  stepperPos[STEPPER_V] = (int)(4096*asin(posY/pendulumDistZ)/(2*PI)/2);
#endif
}
