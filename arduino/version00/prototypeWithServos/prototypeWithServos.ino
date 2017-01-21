#include<Servo.h>
//#include <MegaServo.h>


const int fsmEncoder[4][4]={{0,1,-1,0},{-1,0,0,1},{1,0,0,-1},{0,-1,1,0}};
const uint8_t pinEncoderA=2;
const uint8_t pinEncoderB=3;
unsigned long lastEncoderTime = 0;
unsigned long lastEncoderTimeDiff;
const uint8_t pinServoV=8;
const uint8_t pinServoH=9;

int position=0;
Servo servoH;
Servo servoV;
  
void setup() {
  Serial.begin(9600);
  servoH.attach(pinServoV);
  servoV.attach(pinServoH);
  pinMode(pinEncoderA, INPUT);
  pinMode(pinEncoderB, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinEncoderA), incrementEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinEncoderB), incrementEncoder, CHANGE);
  
}

void loop(){
  //Serial.println(position);
  //Length Pendulum:175mm
  //Length Encoder 75mm
  //Distanze Pendulum 235mm
  double speedAngle = 0 * min(max(lastEncoderTimeDiff/1000,30),-30);
  double angle = 45.0/180.0*(position+speedAngle)*PI/180.0;
  servoH.write(90+180.0/PI*atan(175.0*sin(-angle)/235.0));
  servoV.write(67-35+180.0/PI*atan(175.0*cos(angle)/235.0));
}

void incrementEncoder(){
  static uint8_t lasta = 0;
  static uint8_t lastb = 0;
  uint8_t ina = digitalRead(pinEncoderA)==HIGH;
  uint8_t inb = digitalRead(pinEncoderB)==HIGH;
  //if(ina != lasta || inb != lastb)
  position+=fsmEncoder[(ina<<1)+inb][(lasta<<1)+lastb];
  lasta = ina;
  lastb = inb;
  unsigned long newTime = micros();
  lastEncoderTimeDiff = newTime - lastEncoderTime;
  lastEncoderTime = newTime;
}
