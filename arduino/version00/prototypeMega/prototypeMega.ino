//#include<Servo.h>

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
uint8_t knobEncoderLastState[knobCnt][2]={{0,0},{0,0}};
int32_t knobEncoderPos[knobCnt];

////==Servo==
//const uint8_t pinServoV=8;
//const uint8_t pinServoH=9;
//Servo servoH;
//Servo servoV;

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
int32_t stepperPos[2]={0,0};
int32_t stepperPosDelta[2]={0,0};
int32_t stepperPosReal[2]={0,0};
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
  
void setup() {  
  //==DebugPin==
  pinMode(debugPin,OUTPUT);
  
  //==Laser==
  pinMode(pinLaser,OUTPUT);
  analogWrite(pinLaser,127);
  TCCR1B = (TCCR1B & ~0b111) | 2;
  //Setting   Divisor   Frequency
  //0x01    1     31372.55
  //0x02    8     3921.16
  //0x03      64    490.20   <--DEFAULT
  //0x04      256     122.55
  //0x05    1024    30.64
  //TCCR1B = (TCCR1B & 0b11111000) | <setting>;
  
  //==LEDs==
  for(uint8_t i = 0; i < ledCnt;i++){
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  
  //==Magnet==
  pinMode(pinMagnet[0], OUTPUT);
  pinMode(pinMagnet[1], OUTPUT);
  pinMode(pinMagnetPwm, OUTPUT);
  TCCR2B = (TCCR2B & 0b11111000) | 1;
  /*
  Setting   Divisor   Frequency
  0x01    1     31372.55
  0x02    8     3921.16
  0x03      64    490.20   <--DEFAULT
  0x04      256     122.55
  0x05    1024    30.64
  TCCR1B = (TCCR1B & 0b11111000) | <setting>;
  */

  /*
  //Test Magnet Code
  //digitalWrite(pinMagnetPwm,HIGH);
  while(1){
    setMagnet(true,false,200); //Push
    delay(1000);
    setMagnet(true,true,200); //Push
    delay(1000);
  }
  */
  
  /*
  //==Example Stepper Code==
  for(int i = 0; i < stepperPinCnt; i++){
    pinMode(stepperPins[i], OUTPUT);
  }
  while(1){
    stepperPos++;
    updateStepper();
    delayMicroseconds(1500);
  }
  */
  
  Serial.begin(9600);
  //==Encoder==
  pinMode(pinEncoderA, INPUT);
  pinMode(pinEncoderB, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinEncoderA), incrementEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinEncoderB), incrementEncoder, CHANGE);
  ////==Servo==
  //servoH.attach(pinServoV);
  //servoV.attach(pinServoH);
  //==Stepper==
  for(int i = 0; i < stepperPinCnt; i++){
    pinMode(stepperPins[0][i], OUTPUT);
    pinMode(stepperPins[1][i], OUTPUT);
  }
}

uint32_t loopCnt = 0;
void loop(){
  loopCnt++;
  /*
  //Serial.println(position);
  //Length Pendulum:175mm
  //Length Encoder 75mm
  //Distanze Pendulum 235mm
  double speedAngle = 0 * min(max(lastEncoderTimeDiff/1000,30),-30);
  double angle = 45.0/180.0*(encoderPos+speedAngle)*PI/180.0;
  servoH.write(90+180.0/PI*atan(175.0*sin(-angle)/235.0));
  servoV.write(67-35+180.0/PI*atan(175.0*cos(angle)/235.0));
  */

  if(loopCnt % 10 == 0){
    updateKnobEncoder(0);
    updateKnobEncoder(1);
    stepperPosDelta[0] += knobEncoderPos[0];
    stepperPosDelta[1] += knobEncoderPos[1];
    knobEncoderPos[0] = 0;
    knobEncoderPos[1] = 0;
  }
  readPhotodiodes();
  

  int16_t diodeLU = max(0,(int16_t)photoDiodeValueAfter[PD_LEFT_UP]-(int16_t)photoDiodeValueBefore[PD_LEFT_UP]);
  int16_t diodeLD = max(0,(int16_t)photoDiodeValueAfter[PD_LEFT_DOWN]-(int16_t)photoDiodeValueBefore[PD_LEFT_DOWN]);
  int16_t diodeRU = max(0,(int16_t)photoDiodeValueAfter[PD_RIGHT_UP]-(int16_t)photoDiodeValueBefore[PD_RIGHT_UP]);
  int16_t diodeRD = max(0,(int16_t)photoDiodeValueAfter[PD_RIGHT_DOWN]-(int16_t)photoDiodeValueBefore[PD_RIGHT_DOWN]);
  int16_t horizontal = diodeLU+diodeLD-diodeRU-diodeRD;
  int16_t vertical = diodeLU+diodeRU-diodeLD-diodeRD;
  static double lowHor = 0;
  const uint8_t lowPassGrade = 1;
  const uint8_t triggerLevel = 250;
  const uint8_t triggerLevelHigh = 500;
  static uint8_t verDir = 0;
  lowHor = (lowHor*lowPassGrade+horizontal)/(lowPassGrade+1);
  static double lowVer = 0;
  lowVer = (lowVer*lowPassGrade+vertical)/(lowPassGrade+1);
  
  if(loopCnt % (lowPassGrade+1) == 0){
  if(lowVer > (verDir==1 ? triggerLevel : triggerLevelHigh)){
    stepperPos[STEPPER_V]++;
    verDir = 1;
  }else if(lowVer < -(verDir==0 ? triggerLevel : triggerLevelHigh)){
    stepperPos[STEPPER_V]--;
    verDir = 0;
  }
  
  if(lowHor > triggerLevel){
    stepperPos[STEPPER_H]++;
  }else if(lowHor < -triggerLevel){
    stepperPos[STEPPER_H]--;
  }
  }
  /*
  if(loopCnt % 1000 == 0){
    Serial.print("LU ");
    Serial.print(diodeLU);
    Serial.print(" RU ");
    Serial.println(diodeRU);
    Serial.print("LD ");
    Serial.print(diodeLD);
    Serial.print(" RD ");
    Serial.println(diodeRD);
    Serial.println();
    Serial.print(lowVer);
    Serial.println(lowHor);
    Serial.println();
    Serial.println();
  }
  */
  
  updateStepper(0);
  updateStepper(1);
  
  //delayMicroseconds(10);
}


//==PhotoDiodes==
void readPhotodiodes(){
  //digitalWrite(pinLaser,LOW);
  //delayMicroseconds(2);
  for(int i = 0; i < photoDiodeCnt; i++){
    photoDiodeValueBefore[i] = analogRead(photoDiodePin[i]);
  }
  digitalWrite(pinLaser,HIGH);
  delayMicroseconds(50);
  digitalWrite(debugPin,HIGH);
  for(int i = 0; i < photoDiodeCnt; i++){
    photoDiodeValueAfter[i] = analogRead(photoDiodePin[i]);
  }
  digitalWrite(debugPin,LOW);
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
void updateStepper(uint8_t stepper){
  int32_t pos = stepperPos[stepper] + stepperPosDelta[stepper];
  if(pos > stepperPosReal[stepper]){
    pos--;
  }else if(pos < stepperPosReal[stepper]){
    pos++;
  }else{
    return;
  }
  int stepperState = pos % stepperStateCnt;
  if(stepperState < 0){
    stepperState += stepperStateCnt;
  }
  for(int i = 0; i < stepperPinCnt; i++){
    digitalWrite(stepperPins[stepper][i],fsmStepper[stepperState][i]);
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
  //if(ina != lasta || inb != lastb)
  knobEncoderPos[nr]+=fsmEncoder[(ina<<1)+inb][(lasta<<1)+lastb];
  knobEncoderLastState[nr][0] = ina;
  knobEncoderLastState[nr][1] = inb;
}

void incrementEncoder(){
  static uint8_t lasta = 0;
  static uint8_t lastb = 0;
  uint8_t ina = digitalRead(pinEncoderA)==HIGH;
  uint8_t inb = digitalRead(pinEncoderB)==HIGH;
  //if(ina != lasta || inb != lastb)
  encoderPos+=fsmEncoder[(ina<<1)+inb][(lasta<<1)+lastb];
  lasta = ina;
  lastb = inb;
  
  unsigned long newTime = micros();
  lastEncoderTimeDiff = newTime - lastEncoderTime;
  lastEncoderTime = newTime;
  //stepperPos[0]=-(int)(encoderPos/2);
  //stepperPos[1]= -abs((int)(encoderPos/4));

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
      setMagnet(1,true,176);
      magnetDir = true;
    } else if(((encoderPos < 0) != (maximum < 0)) && magnetDir){
      magnetDir = false;
      setMagnet(0,false,0);
    }
  }
}
