
//==Encoder==
const int8_t fsmEncoder[4][4]={{0,1,-1,0},{-1,0,0,1},{1,0,0,-1},{0,-1,1,0}};

//==KnobEncoder==
const uint8_t knobCnt = 2;
const uint8_t knobEncoderPin[knobCnt][2]={{22,24},{23,25}};
const uint8_t knobEncoderButtonPin[knobCnt]={26,27};
uint8_t knobEncoderLastState[knobCnt][2]={{0,0},{0,0}};
int32_t knobEncoderPos[knobCnt];

//==Stepper==
const int stepperPinCnt = 6;
const int stepperPins[2][stepperPinCnt]={{28,30,32,34,2,3},{29,31,33,35,4,5}};
volatile int32_t stepperPos[2]={0,0};
volatile int32_t stepperPosDelta[2]={0,0};
volatile int32_t stepperPosReal[2]={0,0};
#define STEPPER_H 0
#define STEPPER_V 1
#define STEPPER_UNIPOLAR 0
#define STEPPER_BIPOLAR 1


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
  Serial.begin(9600);
  for(int i = 0; i < stepperPinCnt; i++){
    pinMode(stepperPins[0][i], OUTPUT);
    pinMode(stepperPins[1][i], OUTPUT);
  }
  analogWriteResolution(8);
  stepperSpeedTest();
}

void loop() {
  //900-1200
  stepperPosDelta[STEPPER_V] = 100000;
  if(knobEncoderPos[0] != 0){
    static uint32_t frequency = 900;
    frequency += knobEncoderPos[0];
    knobEncoderPos[0] = 0;
    startTimer(TC1, 0, TC3_IRQn, frequency);
    Serial.println(frequency);
  }
  delay(1);
}

void stepperSpeedTest(){
  //uint16_t frequency = 8000; //Max: 12000
  uint16_t frequency = 40; //Max: 12000
  stepperPos[STEPPER_V] = stepperPos[STEPPER_H] = 2000000000;
  startTimer(TC1, 0, TC3_IRQn, frequency);
  while(1){
    if(knobEncoderPos[0] != 0 || knobEncoderPos[1] != 0){
      frequency += knobEncoderPos[0] * 10;
      frequency += knobEncoderPos[1];
      knobEncoderPos[0] = 0;
      knobEncoderPos[1] = 0;
      startTimer(TC1, 0, TC3_IRQn, frequency);
      Serial.println(frequency);
    }
  }
}

void TC3_Handler()
{
  TC_GetStatus(TC1, 0);
  updateStepper(STEPPER_H);
}
const uint8_t microStepDivision = 64;
const uint8_t vorwardReverse[2][2] = {
  {0,1},
  {1,0}
};
void updateStepper(uint8_t stepper){
  int32_t pos = stepperPos[stepper] + stepperPosDelta[stepper];
  if(pos > stepperPosReal[stepper]){
    stepperPosReal[stepper]++;
  }else if(pos < stepperPosReal[stepper]){
    stepperPosReal[stepper]--;
  }else{
    return;
  }
  int stepperState = stepperPosReal[stepper] % microStepDivision;
  double angle = stepperState*2*PI/microStepDivision;
  double coil[] = {cos(angle), sin(angle)};
  uint8_t maxDuty = 255;
  uint8_t coilDuty[] = {abs(round(maxDuty*coil[0])), abs(round(maxDuty*coil[1]))};

  for(int i = 0; i < 4; i++){
    digitalWrite(stepperPins[stepper][i],vorwardReverse[i%2][coil[i/2]>0]);
//    Serial.print(vorwardReverse[i%2][coil[i/2]>0]);
//    Serial.print(" ");
  }
  analogWrite(stepperPins[stepper][4],coilDuty[0]);
  analogWrite(stepperPins[stepper][5],coilDuty[1]);
//  Serial.print(coilDuty[0]);
//  Serial.print(" ");
//  Serial.print(coilDuty[1]);
//  Serial.println();
  
  
  
//  int stepperState = stepperPosReal[stepper] % stepperStateCnt;
//  if(stepperState < 0){
//    stepperState += stepperStateCnt;
//  }
//  for(int i = 0; i < stepperPinCnt; i++){
//    digitalWrite(stepperPins[stepper][i],fsmStepper[1][stepperState][i]);
//  }
}

